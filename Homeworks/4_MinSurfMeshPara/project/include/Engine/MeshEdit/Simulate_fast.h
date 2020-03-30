#pragma once

#include <Basic/HeapObj.h>
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include <UGM/UGM>
#include <map>
#include <set>
#include <Eigen/Cholesky>

namespace Ubpa {
	class Simulate : public HeapObj {
	public:
		Simulate(const std::vector<pointf3>& plist,
			const std::vector<unsigned>& elist) {
			edgelist = elist;
			this->positions.resize(plist.size());
			this->xk.resize(plist.size());
			this->xk_1.resize(plist.size());

			for (int i = 0; i < plist.size(); i++)
			{
				for (int j = 0; j < 3; j++)
				{
					this->positions[i][j] = plist[i][j];
				}
			}
		};
	public:
		static const Ptr<Simulate> New(const std::vector<pointf3>& plist,
			const std::vector<unsigned> &elist) {
			return Ubpa::New<Simulate>(plist, elist);
		}
	public:
		void Clear();

		// init cache data (eg. half-edge structure) for Run()
		bool Init();
		bool Run();
		
		const std::vector<pointf3>& GetPositions() const { return positions; };

		const double GetStiff() { return stiff; };
		void SetStiff(double k) { stiff = k; Init();};
		const double GetTimeStep() { return h; };
		void SetTimeStep(double k) { h = k; Init();};
		std::vector<unsigned>& GetFix() { return this->fixed_id; };
		void SetFix(const std::vector<unsigned>& f) { this->fixed_id = f; Init();};
		// const std::vector<pointf3>& GetVelocity() { return velocity; };

		void SetLeftFix();


	private:
		void SimulateOnce();

	private:
		double h = 0.03f; 
		double stiff;
		std::vector<unsigned> fixed_id;  //fixed point id

		//mesh data
		std::vector<unsigned> edgelist;

		//simulation data
		std::vector<pointf3> positions;
		// std::vector<pointf3> velocity;

		
	private:
		double g;
		double h;
		size_t m, s;
		size_t iteration;
		Eigen::VectorXd f_ext;
		Eigen::MatrixXd M;
		Eigen::MatrixXd l;
		Eigen::VectorXd x, y;
		Eigen::VectorXd d;
		Eigen::MatrixXd J, L;
		Eigen::MatrixXd K;
		Eigen::MatrixXd A;

		Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > LLT_;
		std::set<unsigned> fix;


	public:
		void FixPoint();
		void UpdatePos();
		void SetFix();
		void buildK();
		void buildL();
		void buildJ();
		void local();
		void global();
		void getb();
	};
}
