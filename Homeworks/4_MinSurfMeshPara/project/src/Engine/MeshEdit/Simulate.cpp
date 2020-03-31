#include <Engine/MeshEdit/Simulate.h>
#include <windows.h>
#include <math.h>
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/Dense>

using namespace Ubpa;
using namespace std;
using namespace Eigen;

void Simulate::SetFast(){
	isfast = true;
}

void Simulate::Clear() {
	this->positions.clear();
}

bool Simulate::Init() {
	isfast = true;
	m = positions.size(); // number of vertices
	s = edgelist.size() / 2;  // number of springs
	g = 9.8;
	iteration = 10;
	stiff = 1e5;

	this->velocity.resize(positions.size());
	for (int i = 0; i < positions.size(); i++)
		for (int j = 0; j < 3; j++)
			this->velocity[i][j] = 0;

	// init l
	for (int i = 0; i < s; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];
		pointf3 v1 = positions[index1];
		pointf3 v2 = positions[index2];
		l.push_back((v1 - v2).norm());
	}

	mass.resize(3 * positions.size());
	f_int.resize(3 * positions.size());
	for (int i = 0; i < 3 * positions.size(); i++) {
		f_int[i] = 0.0;
		mass[i] = 1;
	}

	x.resize(3 * m); // vector x, initialized to be y
	y.resize(3 * m); // vector y = 2q_n - q_n-1
	x_pre.resize(3 * m);
	for (int i = 0; i < m; i++) {
		x.segment(3 * i, 3) << positions[i][0], positions[i][1], positions[i][2];
		y.segment(3 * i, 3) = x.segment(3 * i, 3);
		x_pre.segment(3 * i, 3) = x.segment(3 * i, 3);
	}
	
	// init Mass
	M = MatrixXd::Identity(m * 3, m * 3);

	// init f_ext, i.e. gravity
	f_ext.resize(3 * m);
	for (int i = 0; i < m; i++)
		f_ext.segment(3 * i, 3) = Vector3d(0, -mass[i] * g, 0);

	L = MatrixXd::Zero(m * 3, m * 3);
	buildL();
	J = MatrixXd::Zero(m * 3, s * 3);
	buildJ();

	FixPoint();
	buildK();
	getb();

	// prefactorization
	MatrixXd A_;
	A_.resize(K.rows(), K.rows());
	A_ = K * (M + h * h * L) * K.transpose();

	A = A_.sparseView();
	LLT_.compute(A);

	return true;
}

void Simulate::SetLeftFix() {
	// 固定网格x坐标最小点
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

void Simulate::FixPoint() {
	fixed_id.push_back(10);
 	fixed_id.push_back(120);
}

void Simulate::buildK() {
	K = MatrixXd::Zero(m * 3 - 3 * fixed_id.size(), m * 3);
	set<int> fix(fixed_id.begin(), fixed_id.end());
	for (int i = 0, j = 0; i < m * 3; i++) {
		if (fix.find(i / 3) == fix.end()) {
			K(j++, i) = 1;
		}
	}
}

void Simulate::buildL() {
	// L is 3m * 3m matrix
	MatrixXd temp = MatrixXd::Zero(m, m);

	for (size_t i = 0; i < s; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];
		VectorXd Ai = VectorXd::Zero(m);
		Ai(index1) += 1;
		Ai(index2) -= 1;
		temp += stiff * Ai * Ai.transpose();
	}

	// kronecker product, L = kronecker(temp, I3)
	Matrix3d I3 = Matrix3d::Identity();
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
			L.block(i * 3, j * 3, 3, 3) = temp(i, j) * I3;
}

void Simulate::buildJ() {
	// J is 3m * 3s matrix
	MatrixXd temp = MatrixXd::Zero(m, s);

	for (size_t i = 0; i < s; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];
		VectorXd Ai = VectorXd::Zero(m);
		VectorXd Si = VectorXd::Zero(s);
		Ai(index1) += 1;
		Ai(index2) -= 1;
		Si(i) += 1;
		temp += stiff * Ai * Si.transpose();
	}

	// kronecker product, J = kronecker(temp, I3)
	Matrix3d I3 = Matrix3d::Identity();
	for (int i = 0; i < m; i++)
		for (int j = 0; j < s; j++)
			J.block(i * 3, j * 3, 3, 3) = temp(i, j) * I3;
}

void Simulate::local() {
	d = VectorXd::Ones(s * 3);

	for (size_t i = 0; i < s; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];
		Vector3d p1 = x.segment(3 * index1, 3);
		Vector3d p2 = x.segment(3 * index2, 3);

		d.segment(3 * i, 3) = l[i] * (p1 - p2) / (p1 - p2).norm();
	}

	cout << "Local Step" << endl;
}

void Simulate::global() {
	VectorXd RHS = K * (h * h * J * d + M * y + h * h * f_ext - (M + h * h * L) * b);
	VectorXd xf = LLT_.solve(RHS);
	x = K.transpose() * xf + b;
	cout << "Global Step" << endl;
}

void Simulate::getb() {
	b = x - K.transpose() * K * x;
}

void Simulate::UpdatePos() {
	for (int i = 0; i < m * 3; i++)
		positions[i / 3][i % 3] = x(i);
}

void Simulate::SimulateOnce() {
	if (!isfast) {
		buildX();
		buildV();
	} else {
		//update y, y = 2q_n - q_n-1
		y = 2 * x - x_pre;
		x_pre = x;
		size_t step = 0;
		while (step++ < iteration) {
			local();
			global();
		}
	}
	UpdatePos();
	cout << "Simulate Once" << endl;
}

bool Simulate::Run() {
	SimulateOnce();
	return true;
}

void Simulate::buildX() {
	std::vector<double> y(m * 3);
	for (int i = 0; i < x.size(); i++)
		y[i] = x(i) + h * velocity[i / 3][i % 3] + h * h / mass[i] * f_ext[i];
	xk = y;
	int i = 0;
	do {
		CalForce();
		GetGX();
		CalDiff();
		CalGxM();
		Eigen::MatrixXd t = G_inverse * gx_m;
		xk_1.clear();
		xk_1.resize(gx.size());
		for (int i = 0; i < gx.size(); i++)
			xk_1[i] = xk[i] - t(i, 0);
		i++;
		UpdateX();
		xk.resize(3 * m);
		for (int i = 0; i < m * 3; i++)
			xk[i] = x(i);
	} while (!isConv() && i <= 10);
}

void Simulate::UpdateX() {
	Eigen::MatrixXd xt;
	xt.resize((xk_1.size()), 1);
	for (int i = 0; i < xk_1.size(); i++)
		xt(i, 0) = xk_1[i];

	Eigen::MatrixXd t = K.transpose() * xt;
	for (int i = 0; i < x.size(); i++)
		x(i) = t(i, 0) + b[i];
}

void Simulate::CalGxM() {
	gx_m.resize(gx.size(), 1);
	for (int i = 0; i < gx.size(); i++)
		gx_m(i, 0) = gx[i];
}

bool Simulate::isConv() {
	double delta = 0.01;
	std::vector<double> zero(gx.size(), delta);
	for (int i = 0; i < gx.size(); i++)
		if (abs(gx[i]) > zero[i])
			return false;
	return true;
}

void Simulate::GetGX() {
	std::vector<double> y(xk.size());
	gx.resize(xk.size());

	for (int i = 0; i < xk.size(); i++)
		y[i] = xk[i] + h * velocity[i / 3][i % 3] + h * h / mass[i] * f_ext[i];

	for (int i = 0; i < xk.size(); i++)
		gx[i] = mass[i] * (xk[i] - y[i]) - h * h * f_int[i];

	Eigen::MatrixXd t(gx.size(), 1);
	for (int i = 0; i < gx.size(); i++)
		t(i, 0) = gx[i];

	t = K * t;

	gx.clear();
	gx.resize(t.rows());
	for (int i = 0; i < gx.size(); i++)
		gx[i] = t(i, 0);

	Eigen::MatrixXd xt;
	xt.resize((xk.size()), 1);

	for (int i = 0; i < xk.size(); i++)
		xt(i, 0) = xk[i];

	t = K * xt;
	xk.clear();
	xk.resize(t.rows());
	for (int i = 0; i < xk.size(); i++)
		xk[i] = t(i, 0);
}

void Simulate::CalDiff() {
	std::vector<Eigen::Triplet<double> > triple;
	Eigen::MatrixXd I = MatrixXd::Identity(3, 3);
	for (int i = 0; i < s; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];
		pointf3 v1 = pointf3(x(3 * index1 + 0), x(3 * index1 + 1), x(3 * index1 + 2));
		pointf3 v2 = pointf3(x(3 * index2 + 0), x(3 * index2 + 1), x(3 * index2 + 2));
		vecf3 r = v1 - v2;
		Vector3d t;
		t << r[0], r[1], r[2];

		Eigen::MatrixXd dif;
		dif.resize(3, 3);
		dif = stiff * (l[i] / r.norm() - 1) * I - stiff * l[i]
			/ ((r.norm()) * (r.norm()) * (r.norm())) * t * t.transpose();
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++) {
				triple.push_back(Eigen::Triplet<double>(3 * index1 + j, 3 * index1 + k, -h * h * dif(j, k)));
				triple.push_back(Eigen::Triplet<double>(3 * index2 + k, 3 * index2 + j, -h * h * dif(j, k)));
			}
	}

	for (int i = 0; i < m * 3; i++)
		triple.push_back(Eigen::Triplet<double>(i, i, mass[i]));

	Eigen::MatrixXd diff_;
	Eigen::SparseLU<Eigen::SparseMatrix<double>> LU_;

	diff.setZero();
	diff.resize(m * 3, m * 3);
	diff.setFromTriplets(triple.begin(), triple.end());

	diff_ = K * diff * K.transpose();
	diff = diff_.sparseView();

	I = MatrixXd::Identity(gx.size(), gx.size());
	LU_.analyzePattern(diff);
	LU_.factorize(diff);
	G_inverse = LU_.solve(I);
}

void Simulate::buildV() {
	for (int i = 0; i < x.size(); i++)
		velocity[i / 3][i % 3] = (x(i) - positions[i / 3][i % 3]) / h;
}

void Simulate::CalForce() {
	f_int.clear();
	f_int.resize(3 * positions.size());
	for (int i = 0; i < edgelist.size() / 2; i++) {
		size_t index1 = edgelist[2 * i];
		size_t index2 = edgelist[2 * i + 1];
		pointf3 v1 = pointf3(x(3 * index1 + 0), x(3 * index1 + 1), x(3 * index1 + 2));
		pointf3 v2 = pointf3(x(3 * index2 + 0), x(3 * index2 + 1), x(3 * index2 + 2));
		vecf3 r = v1 - v2;
		for (int j = 0; j < 3; j++) {
			f_int[3 * index1 + j] += -stiff * (r.norm() - l[i]) * r[j] / r.norm();
			f_int[3 * index2 + j] += stiff * (r.norm() - l[i]) * r[j] / r.norm();
		}
	}
}