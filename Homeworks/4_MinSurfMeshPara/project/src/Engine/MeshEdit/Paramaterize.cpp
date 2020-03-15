#include <Engine/MeshEdit/Paramaterize.h>

#include <Engine/MeshEdit/MinSurf.h>

#include <Engine/Primitive/TriMesh.h>

using namespace Ubpa;
using namespace std;

const double MIN = 0.000001;

Paramaterize::Paramaterize(Ptr<TriMesh> triMesh) 
	: heMesh(make_shared<HEMesh<V>>()) {
	Init(triMesh);
}

void Paramaterize::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}

bool Paramaterize::Init(Ptr<TriMesh> triMesh) {
	Clear();
	tex_ = false;
	type_ = 1;

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
			"\t""trimesh is not a triangle mesh or does not have boundariers\n");
		heMesh->Clear();
		return false;
	}

	// triangle mesh's positions ->  half-edge structure's positions
	for (int i = 0; i < nV; i++) {
		auto v = heMesh->Vertices().at(i);
		v->pos = triMesh->GetPositions()[i].cast_to<vecf3>();
	}

	this->triMesh = triMesh;

	interiorV.clear();
	boundaryV.clear();
	new_index.clear();
	old_index.clear();


	return true;
}

bool Paramaterize::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::MinSurf::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	Parameterize();

	// half-edge structure -> triangle mesh
	size_t nV = heMesh->NumVertices();
	size_t nF = heMesh->NumPolygons();
	vector<pointf3> positions;
	vector<pointf2> texcoords;

	vector<unsigned> indice;
	positions.reserve(nV);
	texcoords.reserve(nV);
	indice.reserve(3 * nF);
	for (auto v : heMesh->Vertices()) {
		positions.push_back(v->pos.cast_to<pointf3>());
		texcoords.push_back(v->pos.cast_to<pointf2>());
	}

	for (auto f : heMesh->Polygons()) { // f is triangle
		for (auto v : f->BoundaryVertice()) // vertices of the triangle
			indice.push_back(static_cast<unsigned>(heMesh->Index(v)));
	}
	if (tex_)
		triMesh->Update(texcoords);
	else
		triMesh->Update(positions);

	return true;
}

void Paramaterize::Parameterize() {
	getBoundaryV();
	getInteriorV();

	fixBoundary();

	if (type_ == 1)
		unionWeight();
	else
		cotWeight();

	LU_.compute(L_);
	X = LU_.solve(delta_);

	for (int i = 0; i < interiorV.size(); i++) {
		V* v = interiorV[i];
		v->pos = vecf3({ X(i, 0), X(i, 1), X(i, 2) });
	}
		
}

void Paramaterize::fixBoundary() {
	size_t whole = boundaryV.size();
	size_t half = boundaryV.size() / 2;
	size_t quarter = boundaryV.size() / 4;

	for (size_t i = 0; i < quarter; i++) {
		V* v = boundaryV[i];
		v->pos = vecf3({ i * 1.0 / quarter, 0.0, 0.0 });
	}

	for (size_t i = quarter; i < half; i++) {
		V* v = boundaryV[i];
		v->pos = vecf3({ 1.0, 1.0 * (i - quarter) / (half - quarter), 0.0 });
	}

	for (size_t i = half; i < 3 * quarter; i++) {
		V* v = boundaryV[i];
		v->pos = vecf3({ 1.0 * (3 * quarter - i) / (3 * quarter - half), 1.0, 0.0 });
	}

	for (size_t i = 3 * quarter; i < whole; i++) {
		V* v = boundaryV[i];
		v->pos = vecf3({ 0.0, (whole - i) * 1.0 / (whole - 3 * quarter), 0.0 });
	}
}

void Paramaterize::unionWeight() {
	L_.resize(interiorV.size(), interiorV.size());
	delta_ = Eigen::MatrixX3d::Zero(interiorV.size(), 3);
	vector<Eigen::Triplet<double> > coefficients;

	for (int i = 0; i < interiorV.size(); i++) {
		V* v = interiorV[i];
		double w = 1.0 / (v->Degree());
		coefficients.push_back(Eigen::Triplet<double>(i, i, v->AdjVertices().size()));
		for (auto adj : v->AdjVertices()) {
			int j = heMesh->Index(adj);
			if (!adj->IsBoundary()) {
				j = new_index[j];
				coefficients.push_back(Eigen::Triplet<double>(i, j, -1));
			} else {
				delta_(i, 0) += adj->pos[0];
				delta_(i, 1) += adj->pos[1];
				delta_(i, 2) += adj->pos[2];
			}
		}
	}

	L_.setFromTriplets(coefficients.begin(), coefficients.end());
}

void Paramaterize::cotWeight() {
	L_.resize(interiorV.size(), interiorV.size());
	delta_ = Eigen::MatrixX3d::Zero(interiorV.size(), 3);
	vector<Eigen::Triplet<double> > coefficients;

	for (int i = 0; i < interiorV.size(); i++) {
		V* v = interiorV[i];
		double sum_weight = 0.0;
		int d = v->Degree();
		for (int j = 0; j < d; j++) {
			V* adj = v->AdjVertices()[j];
			int k = heMesh->Index(adj);
			V* adj1 = v->AdjVertices()[(j - 1 + d) % d];
			V* adj2 = v->AdjVertices()[(j + 1) % d];
			double cos1 = abs(vecf3::cos_theta((v->pos - adj1->pos), (adj->pos - adj1->pos)));
			double cos2 = abs(vecf3::cos_theta((v->pos - adj2->pos), (adj->pos - adj2->pos)));
			double sin1 = sqrt(1 - cos1 * cos1);
			double sin2 = sqrt(1 - cos2 * cos2);
			double cot1, cot2;

			if (sin1 < MIN)
				cot1 = sqrt(1.0 / (cos1 * cos1) - 1.0);
			else
				cot1 = cos1 / sin1;
			if (sin2 < MIN)
				cot2 = sqrt(1.0 / (cos2 * cos2) - 1.0);
			else
				cot2 = cos2 / sin2;
			double w = cot1 + cot2;

			sum_weight += w;

			if (!adj->IsBoundary()) {
				k = new_index[k];
				coefficients.push_back(Eigen::Triplet<double>(i, k, -1 * w));
			}
			else {
				delta_(i, 0) += w * adj->pos[0];
				delta_(i, 1) += w * adj->pos[1];
				delta_(i, 2) += w * adj->pos[2];
			}
		}
		coefficients.push_back(Eigen::Triplet<double>(i, i, sum_weight));
	}
	L_.setFromTriplets(coefficients.begin(), coefficients.end());
}

void Paramaterize::getBoundaryV() {
	auto&& boundaries = heMesh->Boundaries();
	for (auto he : boundaries[0])
		boundaryV.push_back(he->Origin());
}

void Paramaterize::getInteriorV() {
	for (auto v : heMesh->Vertices()) {
		if (v->IsBoundary())
			continue;
		size_t size = interiorV.size();
		size_t index = heMesh->Index(v);
		new_index[index] = size;
		old_index[size] = index;
		interiorV.push_back(v);
	}
}

void Paramaterize::SetTex(int m) {
	type_ = m;
	tex_ = true;
}

void Paramaterize::SetType(int m) {
	type_ = m;
}
