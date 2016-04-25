#include "ekf.h"

#include <gsl/gsl_blas.h>
#include <gsl/gsl_sf_gamma.h>
#include <gsl/gsl_linalg.h>

#include <cassert>

#define DT 0.02

EKF::EKF(EKFData* ekfData, unsigned int z_len)
{
	unsigned int x_len, i, j;
	double value;

	m_ekfData = ekfData;

	x_len = m_ekfData->stateVariable().size();

	K = gsl_matrix_alloc(x_len, z_len);
	A = gsl_matrix_alloc(x_len, x_len);
	H = gsl_matrix_alloc(z_len, x_len);
	P = gsl_matrix_alloc(x_len, x_len);
	R = gsl_matrix_alloc(z_len, z_len);	
	Q = gsl_matrix_alloc(x_len, x_len);

	X = gsl_vector_alloc(x_len);
	z = gsl_vector_alloc(z_len);

	/*Init H*/
	vector2matrix(m_ekfData->observationMatrix(), H);

	/*Init P*/
	vector2matrix(m_ekfData->covarianceMatrix(), P);

	/*Init Q And A */
	for( i = 0 ;i < x_len; i++)
	{
		for(j = 0; j < x_len; j++)
		{
			if(i == j)
			{
				value = m_ekfData->processNoiseValue()[i];
			}
			else
			{
				value = 0.0;	
			}

			gsl_matrix_set(A, i, j, 0.0);
			gsl_matrix_set(Q, i, j, value);
		}
	}

	/*Init R*/
	for( i = 0 ;i < z_len; i++)
	{
		for(j = 0; j < z_len; j++)
		{
			if(i == j)
			{
				value = m_ekfData->observationNoiseValue()[i];
			}
			else
			{
				value = 0.0;	
			}

			gsl_matrix_set(R, i, j, value);
		}
	}

	/*Init X*/
	for(i = 0; i < x_len; i++)
	{
		value = m_ekfData->stateVariable()[i];
		gsl_vector_set(X, i, value);
	}
}

EKF::~EKF(void)
{
	gsl_matrix_free(K);
	gsl_matrix_free(A);
	gsl_matrix_free(H);
	gsl_matrix_free(P);
	gsl_matrix_free(R);
	gsl_matrix_free(Q);
       
	gsl_vector_free(X);
	gsl_vector_free(z);
}

void EKF::update(vector<double> z_in)
{
	unsigned int i, j;

	/*时间更新*/

	/*角速度*/
	gsl_vector* wVector = gsl_vector_alloc(3);

	for(i = 0; i < 3; i++)
		gsl_vector_set(wVector, i, gsl_vector_get(X, i+4));

	/*角加速度*/
	gsl_vector* wkVector = gsl_vector_alloc(3);

	for(i = 0; i < 3; i++)
	{
		gsl_vector_set(wkVector, i, gsl_vector_get(X, i+7));

		/*角速度的时间更新*/
		gsl_vector_set(X, i+4, (gsl_vector_get(wVector, i) + gsl_vector_get(wkVector, i)*DT));
	}
	
	/*旋转矢量 rv = (wk + wk + wak*dt)*dt*0.5 */
	
	/*wk + wk*/	
	gsl_blas_dscal(2.0, wVector);
	/*wak*dt*/
	gsl_blas_dscal(DT, wkVector);
	/*wk+wk+wak*dt*/
	gsl_vector_add(wVector, wkVector);
	/*(wk+wk+wak*dt)*dt*0.5*/
	gsl_blas_dscal(DT*0.5, wVector);

	/*四元数的时间更新*/
	/*旋转矩阵*/
	gsl_matrix* M = gsl_matrix_alloc(4, 4);

	gsl_matrix_set(M, 0, 0, 0.0);
	gsl_matrix_set(M, 0, 1, -gsl_vector_get(wVector, 0));
	gsl_matrix_set(M, 0, 2, -gsl_vector_get(wVector, 1));
	gsl_matrix_set(M, 0, 3, -gsl_vector_get(wVector, 2));

	gsl_matrix_set(M, 1, 0,  gsl_vector_get(wVector, 0));
	gsl_matrix_set(M, 1, 1,  0.0);
	gsl_matrix_set(M, 1, 2,  gsl_vector_get(wVector, 2));
	gsl_matrix_set(M, 1, 3, -gsl_vector_get(wVector, 1));

	gsl_matrix_set(M, 2, 0,  gsl_vector_get(wVector, 1));
	gsl_matrix_set(M, 2, 1, -gsl_vector_get(wVector, 2));
	gsl_matrix_set(M, 2, 2,  0.0);
	gsl_matrix_set(M, 2, 3,  gsl_vector_get(wVector, 0));

	gsl_matrix_set(M, 3, 0,  gsl_vector_get(wVector, 2));
	gsl_matrix_set(M, 3, 1,  gsl_vector_get(wVector, 1));
	gsl_matrix_set(M, 3, 3, -gsl_vector_get(wVector, 0));
	gsl_matrix_set(M, 3, 2,  0.0);

	gsl_matrix_scale(M, 0.5);

	gsl_matrix* M_exp = gsl_matrix_alloc(M->size1, M->size2);
	gsl_matrix_memcpy(M_exp, M);
	/*expm(M*dt*0.5) = I + A +A^2/2 + A^3/3*/

	gsl_vector_free(wVector);

	/*四元数*/
	gsl_vector* q = gsl_vector_alloc(4);

	for( i = 0; i < 4; i++)
		gsl_vector_set(q, i, gsl_vector_get(X, i));

	gsl_vector* q_new = gsl_vector_alloc(q->size);

	/*q = expm(M*dt*0.5)*q*/
	gsl_matrix_scale(M_exp, DT);
	expm(M_exp, 10);

	gsl_blas_dgemv(CblasNoTrans,
				   1.0, M_exp, q,
				   0.0, q_new);

	/*四元数时间更新结果写回状态变量X*/
	for( i = 0; i < 4; i++)
		gsl_vector_set(X, i, gsl_vector_get(q_new, i));

	gsl_matrix_free(M_exp);
	gsl_vector_free(q_new);
	gsl_vector_free(q);

	/*构造状态矩阵A*/
	for( i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			gsl_matrix_set(A, i, j, gsl_matrix_get(M, i, j));	
		}
	}

	gsl_matrix_set(A, 7, 4, 1.0);
	gsl_matrix_set(A, 8, 5, 1.0);
	gsl_matrix_set(A, 9, 6, 1.0);

	/*离散化状态矩阵A*/
	gsl_matrix_scale(A, DT);

	expm(A, 10);

	gsl_matrix* temp = gsl_matrix_alloc(P->size1, P->size2);

	/*计算先验协方差*/
	/* P = APA' + Q*/
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
				   1.0, A, P,
				   0.0, temp);

	gsl_blas_dgemm(CblasNoTrans, CblasTrans,
				   1.0, temp, A,
				   0.0, P);

	gsl_matrix_add(P, Q);

	gsl_matrix_free(temp);

	gsl_matrix* temp1 = gsl_matrix_alloc(R->size1, R->size2);
	gsl_matrix* temp5 = gsl_matrix_alloc(P->size1, R->size1);

	/*计算滤波器增益*/
	/*K = P*H'*(H*P*H' + R')^-1*/
	/*P*H'*/
	gsl_blas_dgemm(CblasNoTrans, CblasTrans,
				   1.0, P, H,
				   0.0, temp5);

	/*H*P*H'*/
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
				   1.0, H, temp5,
				   0.0, temp1);

	/*H*P*H' + R*/
	gsl_matrix_add(temp1, R);

	/* inv(H*P*H'+R) */
	gsl_permutation * p = gsl_permutation_alloc(R->size1);

	gsl_matrix* temp2 = gsl_matrix_alloc(R->size1, R->size2);

	int s;

	gsl_linalg_LU_decomp(temp1, p, &s);
	gsl_linalg_LU_invert(temp1, p, temp2);

	/*P*H'*inv(H8P*H'+R)*/
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
				   1.0, temp5, temp2,
				   0.0, K);

	gsl_matrix_free(temp1);
	gsl_matrix_free(temp2);
	gsl_matrix_free(temp5);
	gsl_permutation_free(p);

	/*计算后验估计值*/
	/*X = X + K*(z - H*X)*/
	for( i = 0; i < z->size; i++)
		gsl_vector_set(z, i, z_in[i]);

	/* -H*X  + z */
	gsl_blas_dgemv(CblasNoTrans,
			       -1.0, H, X,
				    1.0, z);

	/* X + K*(z- H*X ) */
	gsl_blas_dgemv(CblasNoTrans,
			        1.0, K, z,
				    1.0, X);


	/*四元数正交化*/
	gsl_vector* norm_q = gsl_vector_alloc(4);

	for(i = 0; i < norm_q->size; i++)
		gsl_vector_set(norm_q, i, gsl_vector_get(X, i));

	double norm_value = gsl_blas_dnrm2(norm_q);

	for(i = 0; i < norm_q->size; i++)
	{
		gsl_vector_set(X, i, gsl_vector_get(norm_q, i)/norm_value);
	}

	gsl_vector_free(norm_q);

	/*计算后验协方差*/
	/*P = (I-K*H)*P*/
	gsl_matrix* I = gsl_matrix_alloc(A->size1, A->size2);

	for(i = 0; i < A->size1; i++)
	{
		for(j = 0; j < A->size2; j++)
		{
			if(i == j)
				gsl_matrix_set(I, i, j, 1.0);
			else
				gsl_matrix_set(I, i, j, 0.0);
		}
	}

	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
			       -1.0, K, H,
				    1.0, I);
	
	gsl_matrix* temp3 = gsl_matrix_alloc(P->size1, P->size2);
	
	gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
					 1.0, I, P,
					 0.0, temp3);

	gsl_matrix_memcpy(P, temp3);

	gsl_matrix_free(I);
	gsl_matrix_free(temp3);
}

void EKF::vector2matrix(vector<double> v, gsl_matrix* m)
{
	unsigned int i, j;

	assert(v.size() == (m->size1*m->size2));

	for(i = 0; i < m->size1; i++)
	{
		for(j = 0; j < m->size2; j++)
			gsl_matrix_set(m, i, j, v[j+ m->size2*i]);
	}
}

void EKF::matrix2vector(vector<double> v, gsl_matrix* m)
{
	unsigned int i, j;

	assert(v.size() == (m->size1*m->size2));

	for(i = 0; i < m->size1; i++)
	{
		for(j = 0; j < m->size2; j++)
			v[j+ m->size2*i] = gsl_matrix_get(m, i, j);
	}
}

void EKF::expm(gsl_matrix* m, unsigned int n)
{
	unsigned int i, j;

	/*必须是方阵*/
	assert(m->size1 == m->size2);

	/*累加器初值为单位矩阵*/
	gsl_matrix* sum   = gsl_matrix_alloc(m->size1, m->size2);
	gsl_matrix* temp  = gsl_matrix_alloc(m->size1, m->size2);
	gsl_matrix* temp1 = gsl_matrix_alloc(m->size1, m->size2);

	/*构造单位矩阵*/
	for(i = 0; i < m->size1; i++)
	{
		for(j = 0; j < m->size2; j++)
		{
			if(i == j)
				gsl_matrix_set(sum, i, j, 1.0);
			else
				gsl_matrix_set(sum, i, j, 0.0);
		}
	}


	gsl_matrix_memcpy(temp, m);

	/*I+A*/
	gsl_matrix_add(sum, temp);

	/* A*A^? */
	for( i = 0; i < n-1; i++)
	{
		gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,
						1.0, temp, m,
						0.0, temp1);

		gsl_matrix_memcpy(temp, temp1);

		gsl_matrix_scale(temp1, (1/(gsl_sf_fact(i+2))));

		gsl_matrix_add(sum, temp1);
	}
	
	gsl_matrix_memcpy(m, sum);

	gsl_matrix_free(sum);
	gsl_matrix_free(temp);
	gsl_matrix_free(temp1);
}

vector<double> EKF::eulerAngles(void)
{
	vector<double> angles;

	double q0 = gsl_vector_get(X, 0);
	double q1 = gsl_vector_get(X, 1);
	double q2 = gsl_vector_get(X, 2);
	double q3 = gsl_vector_get(X, 3);

	angles.resize(2);

	angles[0] = atan2(q0*q0-q1*q1-q2*q2+q3*q3, 2*(q1*q3-q0*q2));
	angles[1] = asin(-2*(q2*q3-q0*q1));

	return angles;
}

void EKF::printfMatrix(const gsl_matrix* m)
{
	unsigned int i, j;

	for(i = 0; i < m->size1; i++)
	{
		for(j = 0; j < m->size2; j++)
			printf("%5.5lf ", gsl_matrix_get(m, i, j));

		printf("\n");
	}
}
