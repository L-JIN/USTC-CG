#include <Engine/MeshEdit/MinSurf.h>
#include <Engine/Primitive/TriMesh.h>
#include <Eigen/Sparse>
#include <unordered_set>

using namespace Ubpa;
using namespace std;
using namespace Eigen;

MinSurf::MinSurf(Ptr<TriMesh> triMesh)
	: heMesh(make_shared<HEMesh<V>>()){
	Init(triMesh);
}

void MinSurf::Clear() {
	heMesh->Clear();
	triMesh = nullptr;
}

bool MinSurf::Init(Ptr<TriMesh> triMesh) {
	Clear();

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
	return true;
}

bool MinSurf::Run() {
	if (heMesh->IsEmpty() || !triMesh) {
		printf("ERROR::MinSurf::Run\n"
			"\t""heMesh->IsEmpty() || !triMesh\n");
		return false;
	}

	Minimize();

	// half-edge structure -> triangle mesh
	size_t nV = heMesh->NumVertices();
	size_t nF = heMesh->NumPolygons();
	vector<pointf3> positions;
	vector<unsigned> indice;
	positions.reserve(nV);
	indice.reserve(3 * nF);
	for (auto v : heMesh->Vertices())
		positions.push_back(v->pos.cast_to<pointf3>());
	for (auto f : heMesh->Polygons()) { // f is triangle
		for (auto v : f->BoundaryVertice()) // vertices of the triangle
			indice.push_back(static_cast<unsigned>(heMesh->Index(v)));
	}

	triMesh->Init(indice, positions);

	return true;
}


void MinSurf::Minimize() {
	// 1. Find the boundary vertexes (get index)
	//    also build a judge set
	unordered_set<V*> boundaryVset;
	auto && boundaries = heMesh->Boundaries();
	for (auto he : boundaries[0]) {
		boundaryVset.insert(he->Origin());
	}

	// 2. find all interior and build modified Laplace matrix L,
	// L is (n-m)x(n-m) with n bein number of vertices and m being number of bondary vertices
	vector<V*> interior_vertices;
	unordered_map<size_t, size_t> new_index, old_index;
	for (auto v : heMesh->Vertices()) {
		if (boundaryVset.find(v) != boundaryVset.end())
			continue;
		size_t size = interior_vertices.size();
		size_t index = heMesh->Index(v);
		new_index[index] = size;
		old_index[size] = index;
		interior_vertices.push_back(v);
	}
	
	// get n and m
	size_t m = boundaryVset.size();
	size_t n = heMesh->NumVertices();

	cout << "m = " << m << " n = " << n << endl;
	L_.resize(n - m, n - m);
	delta_ = Eigen::MatrixX3d::Zero(n - m, 3);

	// build modified uniform Laplace Matrix (without boundary vertices) and delta
	vector<Eigen::Triplet<double>> coefficients;
	for (auto v : interior_vertices) {
		size_t i = heMesh->Index(v);
		i = new_index[i];
		coefficients.emplace_back(i, i, v->AdjVertices().size());

		for (auto adj : v->AdjVertices()) {
			size_t j = heMesh->Index(adj);
			if (boundaryVset.find(adj) == boundaryVset.end()) {
				j = new_index[j];
				coefficients.emplace_back(i, j, -1);
			}else {
				delta_(i, 0) += adj->pos[0];
				delta_(i, 1) += adj->pos[1];
				delta_(i, 2) += adj->pos[2];
			}
		}
	}

	// solve the linear system
	L_.setFromTriplets(coefficients.begin(), coefficients.end());
	
	LU_.analyzePattern(L_);
	LU_.factorize(L_);
	Eigen::MatrixX3d X = LU_.solve(delta_);

	// 3. update vertexes
	vector<pointf3> new_vertexes;
	new_vertexes.resize(n);
	for (auto v : interior_vertices) {
		size_t index = heMesh->Index(v);
		size_t i = new_index[index];
		v->pos = vecf3({ X(i, 0), X(i, 1), X(i, 2) });
	}
	
}
