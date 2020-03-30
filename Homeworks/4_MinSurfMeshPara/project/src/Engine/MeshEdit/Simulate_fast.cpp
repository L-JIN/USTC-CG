#include <Engine/MeshEdit/Simulate.h>

#include <math.h>
#include <Eigen/Core>
#include <Eigen/Sparse>

using namespace Ubpa;

using namespace std;
using namespace Eigen;

void Simulate::Clear() {
	this->positions.clear();
}

bool Simulate::Init() {
	m = positions.size() / 3; // number of vertices
	s = edgelist.size() / 2;  // number of springs
	g = 9.8;
	h = 1 / 30;
	iteration = 10;

	// init q_n-1, q_n, q_n+1
	x.resize(3 * m); // vector x, initialized to be y
	y.resize(3 * m); // vector y = 2q_n - q_n-1
	for (int i = 0; i < m; i++) {
		x.segment(3 * i, 3) << positions[i][0]
							<< positions[i][1]
							<< positions[i][2];
		y.segment(3 * i, 3) = x.segment(3 * i, 3);
	}

	// init Mass Matrix
	M.resize(3 * m);
	M.setOnes();

	// init force_ext, i.e. gravity
	f_ext.resize(3 * m);
	for (int i = 0; i < m; i++)
		f_ext.segment(3 * i, 3) = Vector3d(0, 0, -1);

	L = MatrixXd::Zero(m * 3, m * 3); 
	buildL();
	J = MatrixXd::Zero(m * 3, m * 3); 
	buildJ();

	Fixpoint();
	buildK();
	getb();

	// prefactorization
	MatrixXd A_ = K * (M + h * h * L) * K.transpose() * K;
	A = A_.sparseView();
	LLT_.compute(A);

	return true;
}

void Simulate::SetLeftFix()
{
	//固定网格x坐标最小点
	fixed_id.clear();
	double x = 100000;
	for (int i = 0; i < positions.size(); i++)
	{
		if (positions[i][0] < x)
		{
			x = positions[i][0];
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		if (abs(positions[i][0] - x) < 1e-5)
		{
			fixed_id.push_back(i);
		}
	}

	Init();
}

void Simulate::Fixpoint() {
	fixed_id.push_back(0);
	fixed_id.push_back(m / 4);
}

void Simulate::buildK() {
	K.resize(m * 3 - 3 * fixed_id.size(), m * 3);
	K.SetZero();

	for (int i = 0, j = 0; i < xk.size(); i++) {
		if (fix.find(i) == fix.end()) {
			K(j++, i) = 1;
		}
	}

	// Eigen::MatrixXd xt;
	// xt.resize((x.size()),1);
	// b.resize(x.size());

	// for (int i = 0; i < x.size(); i++)
	// 	xt(i, 0) = x[i]; 
	// Eigen::MatrixXd t = K.transpose() * K * xt;

	// for (int i = 0; i < xk.size(); i++)
	// 	b[i] = x[i] - t(i, 0);
}

void Simulate::buildL() {
	size_t m = positions.size() / 3;
	size_t s = edgelist.size() / 2

	MatrixXd temp = MatrixXd::Zero(m, m);
	
	for (size_t i = 0; i < s; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];

		VectorXd Ai = VectorXd::Zero(m);
		Ai(index1 - 1) += 1;
		Ai(index2 - 1) -= 1;
		temp += stiff * Ai * Ai.transpose();
	}

	Matrix3d I3 = Matrix3d::Identity();

	// kronecker product, L = kronecker(temp, I3)
	for (int i = 0; i < m; i++) {
    	L.block(i * 3, i * 3, 3, 3) = X(i, i) * I3;
	}
}

void Simulate::buildJ() {
	MatrixXd temp = MatrixXd::Zero(m, s);
	
	for (size_t i = 0; i < s; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];

		VectorXd Ai = VectorXd::Zero(m);
		VectorXd Si = VectorXd::Zero(s);
		Ai(index1 - 1) += 1;
		Ai(index2 - 1) -= 1;
		Si(i) += 1;
		temp += stiff * Ai * Si.transpose();
	}

	Matrix3d I3 = Matrix3d::Identity();

	// kronecker product, L = kronecker(temp, I3)
	for (int i = 0; i < m; i++) {
    	L.block(i * 3, i * 3, 3, 3) = X(i, i) * I3;
	}

}

void Simulate::local() {
	d = VectorXd::Ones(s * 3);

	for (size i = 0; i < s; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];
		pointf3 p1 = positions[index1];
		pointf3 p2 = positions[index2];
		vecf3 r = p1 - p2;

		VectorXd di(3);
		di << r[0], r[1], r[2];
		
		d.segment(3 * i, 3) = l[i] * di / pointf3::distance(p1, p2);
	}

}

void Simulate::global() {
	VectorXd RHS = K * (h * h * J * d + M * y - h * h * f_ext - (M + h * h * L) * b);

	VectorXd xf = LLT_.solve(RHS);

	x = K.transpose() * xf + b;

}

void Simulate::getb() {
	b = x - K.transpose() * K * x;
}

void Simulate::UpdatePos() {
	for (int i = 0; i < m * 3; i++)
		positions[i / 3][i % 3] = x(i);
}

void Simulate::SimulateOnce() {
	//update y, y = 2q_n - q_n-1
	y = 2 * x - y;

	while(step < iteration) {
		local();
		global();
	}
	UpdatePos();
}

void Simulate::Run() {
	SimulateOnce();
}