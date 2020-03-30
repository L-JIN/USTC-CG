#include <Engine/MeshEdit/Para_arap.h>

#include <Engine/MeshEdit/MinSurf.h>

#include <Engine/Primitive/TriMesh.h>

#include <math.h>
#include <map>

using namespace Ubpa;

using namespace std;

Arap::Arap(Ptr<TriMesh> triMesh) :Paramaterize(triMesh), heMesh(make_shared<HEMesh<V>>())
{
	Init(triMesh);
}

void Arap::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}


bool Arap::Init(Ptr<TriMesh> triMesh) {
	cout << (triMesh == nullptr) << endl;
	Clear();
	tex_ = false;
	if (triMesh == nullptr)
		return true;

	if (triMesh->GetType() == TriMesh::INVALID) {
		printf("ERROR::MinSurf::Init:\n"
			"\t""trimesh is invalid\n");
		return false;
	}

	// init half-edge structure
	size_t nV = triMesh->GetPositions().size();
	vector<vector<size_t>> triangles;
	triangles.reserve(triMesh->GetTriangles().size());
	for (auto triangle : triMesh->GetTriangles())
		triangles.push_back({ triangle->idx[0], triangle->idx[1], triangle->idx[2] });
	heMesh->Reserve(nV);
	heMesh->Init(triangles);

	if (!heMesh->IsTriMesh() || !heMesh->HaveBoundary()) {
		printf("ERROR::MinSurf::Init:\n"
			"\t""trimesh is not a triangle mesh or hasn't a boundaries\n");
		heMesh->Clear();
		return false;
	}

	// triangle mesh's positions ->  half-edge structure's positions
	for (int i = 0; i < nV; i++) {
		auto v = heMesh->Vertices().at(i);
		v->pos = triMesh->GetPositions()[i].cast_to<vecf3>();
	}

	this->triMesh = triMesh;

	return true;

}

void Arap::SetPos()
{
	for (auto v : heMesh->Vertices())
	{
		if (v->IsBoundary())
		{
			pos_not_move=heMesh->Index(v);
			return;
		}
	}

	pos_not_move = 0;
}

void Arap::initUt()
{
	auto cotp = Paramaterize::New(triMesh);
	cotp->SetType(2);
	Ptr < HEMesh < V >> init_hemesh = cotp->ReturnPos();

	Ut_.clear();

	Ut_.resize(heMesh->NumHalfEdges());

	int i = -1;

	for (auto p : init_hemesh->Polygons())
	{
		i++;
		auto p2 = heMesh->Polygons()[i];

		if (p != nullptr)
		{
			auto edges = p->BoundaryHEs();
			auto edges2 = p2->BoundaryHEs();

			V* v1 = edges[0]->Origin();
			V* v2 = edges[0]->End();
			V* v3 = edges[0]->Next()->End();

			Ut_[he_to_index[edges2[0]]] = Eigen::Vector2d(v1->pos[0], v1->pos[1]);
			Ut_[he_to_index[edges2[0]->Next()]] = Eigen::Vector2d(v2->pos[0], v2->pos[1]);
			Ut_[he_to_index[edges2[0]->Next()->Next()]] = Eigen::Vector2d(v3->pos[0], v3->pos[1]);
		}
	}
	V* p = init_hemesh->Vertices()[pos_not_move];
	pos_coor = Eigen::Vector2d(p->pos[0],p->pos[1]);

}

bool Arap::Run()
{
	//TODO
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::MinSurf::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	ParaByArap();

	// half-edge structure -> triangle mesh
	size_t nV = heMesh->NumVertices();
	size_t nF = heMesh->NumPolygons();
	vector<pointf3> positions;
	vector<pointf2> texcoords;

	vector<unsigned> indice;
	positions.reserve(nV);
	texcoords.reserve(nV);
	indice.reserve(3 * nF);
	for (auto v : heMesh->Vertices())
	{
		positions.push_back(v->pos.cast_to<pointf3>());
		texcoords.push_back(v->pos.cast_to<pointf2>());
	}

	for (auto f : heMesh->Polygons()) { // f is triangle
		for (auto v : f->BoundaryVertice()) // vertices of the triangle
			indice.push_back(static_cast<unsigned>(heMesh->Index(v)));
	}
	
	triMesh->Update(positions);

	return true;
}

void Arap::SetTex()
{
	tex_ = true;
}

void Arap::ParaByArap()
{
	GetMap();
	CacXtAndCot();
	SetPos();
	initUt();
	
	for (int i = 0; i <= 20; i++)
	{
		CacLt();
		CacB();
		CacCof();

		LU_.compute(Cof_);
		X_ = LU_.solve(b_);

		UpdateUt();
		UpdateHeMesh();
	}
	
}

void Arap::CacXtAndCot()
{
	int edgenum = heMesh->NumHalfEdges();

	Xt_.resize(edgenum);
	Cot_.resize(edgenum);

	for (auto p : heMesh->Polygons())
	{
		if (p != nullptr)
		{
			auto edges = p->BoundaryHEs();
			for (auto e : edges)
			{
				double cot = CacCot(e);
				Cot_[he_to_index[e]] = cot;
			}

			V* v1 = edges[0]->Origin();
			V* v2 = edges[0]->End();
			V* v3 = edges[0]->Next()->End();
			
			Xt_[he_to_index[edges[0]]] = Eigen::Vector2d(0, 0);
			Xt_[he_to_index[edges[1]]] = Eigen::Vector2d((v1->pos-v2->pos).norm(), 0);
			Xt_[he_to_index[edges[2]]] = 
				Eigen::Vector2d((v2->pos - v1->pos).dot(v3->pos - v1->pos), 
				(v2->pos - v1->pos).cross(v3->pos - v1->pos).norm()/ ((v1->pos - v2->pos).norm()));
		}
	}
}

void Arap::GetMap()
{
	int i = 0;
	for (auto e : heMesh->HalfEdges())
	{
		he_to_index.insert(std::pair<THalfEdge<V, E, P>*, int>(e, i));
		i++;
	}
}

double Arap::CacCot(THalfEdge<V, E, P> *he)
{
	V* v1 = he->Origin();
	V* v2 = he->End();
	V* v3 = he->Next()->End();
	
	double cos = vecf3::cos_theta((v1->pos - v3->pos), (v2->pos - v3->pos));
	double sin = sqrt(1 - cos * cos);

	return cos / sin;
}

void Arap::CacLt()
{
	Lt_.clear();

	Lt_.resize(heMesh->NumPolygons());

	for (auto p : heMesh->Polygons())
	{
		Eigen::Matrix2d St;
		St.fill(0);
		if (p != nullptr)
		{
			St.fill(0);

			auto edges = p->BoundaryHEs();

			for (int i=0;i<=2;i++)
			{
				St += Cot_[he_to_index[edges[i]]] * 
					(Ut_[he_to_index[edges[i]]]-Ut_[he_to_index[edges[i]->Next()]])*
					((Xt_[he_to_index[edges[i]]] - Xt_[he_to_index[edges[i]->Next()]]).transpose());
			}
			
			Eigen::JacobiSVD<Eigen::Matrix2d> svd(St, Eigen::ComputeFullU | Eigen::ComputeFullV);
			Lt_[heMesh->Index(p)] = svd.matrixU() * (svd.matrixV().transpose());
		}
	}
}

void Arap::CacB()
{
	b_ = Eigen::MatrixX2d::Zero(heMesh->NumVertices(), 2);

	b_(pos_not_move, 0) = pos_coor[0];
	b_(pos_not_move, 1) = pos_coor[1];


	int i = -1;

	for (auto v : heMesh->Vertices())
	{
		i++;

		int k = heMesh->Index(v);

		if (k == pos_not_move)
			continue;

		Eigen::Vector2d b;
		b = Eigen::Vector2d::Zero();
		for (auto adj:v->AdjVertices())
		{
			auto he1 = v->HalfEdgeTo(adj);
			if (he1 != nullptr)
			{
				auto he1_next = he1->Next();
				auto p1 = he1->Polygon();
				if (p1 != nullptr)
					b += (Cot_[he_to_index[he1]] * Lt_[heMesh->Index(p1)]) *
					(Xt_[he_to_index[he1]] - Xt_[he_to_index[he1_next]]);
			}

			auto he2 = adj->HalfEdgeTo(v);
			if (he2 != nullptr)
			{
				auto he2_next = he2->Next();
				auto p2 = he2->Polygon();
				if (p2 != nullptr)
					b -= (Cot_[he_to_index[he2]] * Lt_[heMesh->Index(p2)]) *
					(Xt_[he_to_index[he2]] - Xt_[he_to_index[he2_next]]);
			}
		}

		b_(k, 0) = b(0);
		b_(k, 1) = b(1);
	}
}


void Arap::CacCof()
{
	Cof_.resize(heMesh->NumVertices(), heMesh->NumVertices());

	double w, sum_w;

	int index = pos_not_move;
	Cof_.insert(index, index) = 1.0;

	int i = -1;
	for (auto v : heMesh->Vertices())
	{	
		i++;
		int k = heMesh->Index(v);
		if (k == pos_not_move)
			continue;

		sum_w = 0;

		for (auto adj : v->AdjVertices())
		{
			w = 0;
			int j = heMesh->Index(adj);

			auto he1 = v->HalfEdgeTo(adj);
			auto he2 = adj->HalfEdgeTo(v);

			auto p1 = he1->Polygon();
			auto p2 = he2->Polygon();

			if (p1 != nullptr)
				w += Cot_[he_to_index[he1]] ;
				
			if (p2 != nullptr)
				w += Cot_[he_to_index[he2]];
				

			sum_w += w;
			Cof_.insert(k, j) = -w;
		}
		Cof_.insert(k, k) = sum_w; 
	}
}

void Arap::UpdateUt()
{
	for (auto p : heMesh->Polygons())
	{
		if (p != nullptr)
		{
			auto edges = p->BoundaryHEs();

			V* v1 = edges[0]->Origin();
			V* v2 = edges[0]->End();
			V* v3 = edges[0]->Next()->End();

			int i1 = heMesh->Index(v1);
			int i2 = heMesh->Index(v2);
			int i3 = heMesh->Index(v3);

			Ut_[he_to_index[edges[0]]] = Eigen::Vector2d(X_(i1, 0), X_(i1, 1));
			Ut_[he_to_index[edges[1]]] = Eigen::Vector2d(X_(i2, 0), X_(i2, 1));
			Ut_[he_to_index[edges[2]]] = Eigen::Vector2d(X_(i3, 0), X_(i3, 1));
		}
	}
}

void Arap::UpdateHeMesh()
{
	for (auto v : heMesh->Vertices())
	{
		int i = heMesh->Index(v);
		v->pos= vecf3({X_(i, 0), X_(i, 1), 0 });
	}
}