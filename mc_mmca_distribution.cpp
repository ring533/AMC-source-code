#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sstream>
#include "TreeNodeV4.h"

#define N_node 1000 //
#define T 401
#define R 30
#define MAX_PROCESS_COUNT 5
int matrix1[N_node][500];
int matrix2[N_node][500];
int degree1[N_node], degree2[N_node];

void save_matrix1(const char *path)
{
	int i, j;
	srand(0); //
	FILE *pf1;
	pf1 = fopen(path, "r");
	if (!pf1)
		printf("can't open file\n");
	i = 0;
	while (i < N_node)
	{
		j = 0;
		for (j = 0; j < N_node; j++)
		{
			fscanf(pf1, "%d ", &matrix1[i][j]);
			if (matrix1[i][j] == -1)
			{
				degree1[i] = j;
				j = 0;
				break;
			}
		}
		i++;
	}
	delete pf1;
}
void save_matrix2(const char *path)
{
	int i, j;
	srand(0);
	FILE *pf2;
	pf2 = fopen(path, "r");
	if (!pf2)
		printf("can't open file\n");
	i = 0;
	while (i < N_node)
	{
		j = 0;
		for (j = 0; j < N_node; j++)
		{
			fscanf(pf2, "%d ", &matrix2[i][j]);
			if (matrix2[i][j] == -1)
			{
				degree2[i] = j;
				j = 0;
				break;
			}
		}
		i++;
	}
	delete pf2;
}
int sum(char *state, char target)
{
	int i, sum = 0;
	for (i = 0; i < N_node; i++)
	{
		if (state[i] == target)
			sum++;
	}
	return sum;
}

void run_mc(double lambda, double delta, double gamma, double beta, double sigma, double mu, double m1, double m21, double m22, double m23, double p1, double p2, double p3, ostream &out)
{
	char state[N_node], state2[N_node];
	char tate[N_node], tate2[N_node];
	double DATA_S[R][T], DATA_I[R][T], DATA_V1[R][T], DATA_V2[R][T], DATA_V3[R][T], DATA_U[R][T], DATA_A[R][T];
	int t, l, j, k;
	int i, s, v1, v2, v3, u, a_num;
	double temp, w1, w2;
	for (l = 0; l < R; l++) // 20
	{
		i = s = v1 = v2 = v3 = 0;
		u = a_num = 0;
		for (j = 0, t = 0; j < N_node; j++) // 0.05;
		{
			temp = rand() * 1.0 / RAND_MAX;
			if (temp < 0.2)
			{
				state[j] = 'i';
				tate[j] = 'a';
				i++; //
				a_num++;
			}
			else
			{
				state[j] = 's';
				tate[j] = 'u';
				s++;
				u++;
			}
		}
		DATA_S[l][t] = s * 1.0 / N_node;
		DATA_I[l][t] = i * 1.0 / N_node;
		DATA_V1[l][t] = v1 * 1.0 / N_node;
		DATA_V2[l][t] = v2 * 1.0 / N_node;
		DATA_V3[l][t] = v3 * 1.0 / N_node;
		DATA_U[l][t] = u * 1.0 / N_node;
		DATA_A[l][t] = a_num * 1.0 / N_node;
		for (t = 1; t < T; t++)
		{

			for (j = 0; j < N_node; j++)
				tate2[j] = tate[j];
			for (j = 0; j < N_node; j++)
			{
				if (tate[j] == 'u')
				{
					int na = 0;
					for (k = 0; k < degree2[j]; k++)
					{
						if (tate[matrix2[j][k]] == 'a')
						{
							na++;
						}
					}
					w1 = 1 - pow((1 - lambda), na);
					temp = 1.0 * rand() / RAND_MAX;
					if (temp < w1)
					{
						tate2[j] = 'a';
					}
				}
				if (tate[j] == 'a')
				{
					temp = 1.0 * rand() / RAND_MAX;
					if (temp < delta)
					{
						tate2[j] = 'u';
					}
				}
			}
			for (j = 0; j < N_node; j++)
			{
				if (tate[j] == 'u' && tate2[j] == 'a')
				{
					tate[j] = 'a';
					u--;
					a_num++;
				} // uau
				else if (tate[j] == 'a' && tate2[j] == 'u')
				{
					tate[j] = 'u';
					u++;
					a_num--;
				}
			}
			for (j = 0; j < N_node; j++)
			{
				if (tate[j] == 'u')
				{
					temp = 1.0 * rand() / RAND_MAX;
					if (temp < m1)
					{
						tate[j] = 'a';
						u--;
						a_num++;
					}
				}
			}
			for (j = 0; j < N_node; j++)
			{
				if (state[j] == 's')
				{
					temp = 1.0 * rand() / RAND_MAX;
					if (temp < m21)
					{
						state[j] = 'v';
						s--;
						v1++;
					}
				}
				else if (state[j] == 'v')
				{
					temp = 1.0 * rand() / RAND_MAX;
					if (temp < m22)
					{
						state[j] = 'b';
						v1--;
						v2++;
					}
				}
				else if (state[j] == 'b')
				{
					temp = 1.0 * rand() / RAND_MAX;
					if (temp < m23)
					{
						state[j] = 'm';
						v2--;
						v3++;
					}
				}
			}
			//************************//
			for (j = 0; j < N_node; j++) // 1sisbetabeta*gamma,mu
				state2[j] = state[j];
			for (j = 0; j < N_node; j++)
			{
				if (state[j] == 's')
				{
					int n = 0; //
					for (k = 0; k < degree1[j]; k++)
					{
						if (state[matrix1[j][k]] == 'i')
						{
							n++;
						}
					}
					if (tate[j] == 'u') // S_U--->I_U
					{
						w2 = 1 - pow((1 - beta), n);
					}
					else // S_A--->I_U
					{
						w2 = 1 - pow((1 - beta * gamma), n);
					}
					temp = rand() * 1.0 / RAND_MAX;
					if (temp < w2)
					{
						state2[j] = 'i';
					}
				}
				else if (state[j] == 'v')
				{
					int n = 0; //
					for (k = 0; k < degree1[j]; k++)
					{
						if (state[matrix1[j][k]] == 'i')
						{
							n++;
						}
					}
					if (tate[j] == 'u') // S_U--->I_U
					{
						w2 = 1 - pow((1 - p1 * beta* gamma), n);
					}
					else // S_A--->I_U
					{
						w2 = 1 - pow((1 - p1 * beta * gamma), n);
					}
					temp = rand() * 1.0 / RAND_MAX;
					if (temp < w2)
					{
						state2[j] = 'i';
					}
				}
				else if (state[j] == 'b')
				{
					int n = 0; //
					for (k = 0; k < degree1[j]; k++)
					{
						if (state[matrix1[j][k]] == 'i')
						{
							n++;
						}
					}
					if (tate[j] == 'u') // S_U--->I_U
					{
						w2 = 1 - pow((1 - p2 * beta* gamma), n);
					}
					else // S_A--->I_U
					{
						w2 = 1 - pow((1 - p2 * beta * gamma), n);
					}
					temp = rand() * 1.0 / RAND_MAX;
					if (temp < w2)
					{
						state2[j] = 'i';
					}
				}
				else if (state[j] == 'm')
				{
					int n = 0; //
					for (k = 0; k < degree1[j]; k++)
					{
						if (state[matrix1[j][k]] == 'i')
						{
							n++;
						}
					}
					if (tate[j] == 'u') // S_U--->I_U
					{
						w2 = 1 - pow((1 - p3 * beta* gamma), n);
					}
					else // S_A--->I_U
					{
						w2 = 1 - pow((1 - p3 * beta * gamma), n);
					}
					temp = rand() * 1.0 / RAND_MAX;
					if (temp < w2)
					{
						state2[j] = 'i';
					}
				}
				else if (state[j] == 'i')
				{
					temp = rand() * 1.0 / RAND_MAX;
					if (temp < mu)
					{
						state2[j] = 's';
					}
				}
			}
			for (j = 0; j < N_node; j++)
				state[j] = state2[j];
			for (j = 0; j < N_node; j++)
			{ // 3sigma
				if (state[j] = 'i' && tate[j] == 'u')
				{
					temp = rand() * 1.0 / RAND_MAX;
					if (temp < sigma)
					{
						tate[j] = 'a';
						u--;
						a_num++;
					}
				}
			}
			for (j = 0; j < N_node; j++)
				state[j] = state2[j];
			i = s = v1 = v2 = v3 = 0;
			i = sum(state, 'i');
			s = sum(state, 's');
			v1 = sum(state, 'v');
			v2 = sum(state, 'b');
			v3 = sum(state, 'm');
			DATA_S[l][t] = s * 1.0 / N_node;
			DATA_I[l][t] = i * 1.0 / N_node;
			DATA_V1[l][t] = v1 * 1.0 / N_node;
			DATA_V2[l][t] = v2 * 1.0 / N_node;
			DATA_V3[l][t] = v3 * 1.0 / N_node;
			DATA_U[l][t] = u * 1.0 / N_node;
			DATA_A[l][t] = a_num * 1.0 / N_node;
		}
	}
	// 输出
	double c_i, c_s, c_v1, c_v2, c_v3, c_u, c_a;
	int mean_time = 10;
	c_i = c_s = c_v1 = c_v2 = c_v3 = c_u = c_a = 0;
	for (t = T - mean_time; t < T; t++)
	{
		for (l = 0; l < R; l++)
		{
			c_a += DATA_A[l][t];
			c_u += DATA_U[l][t];
			c_s += DATA_S[l][t];
			c_i += DATA_I[l][t];
			c_v1 += DATA_V1[l][t];
			c_v2 += DATA_V2[l][t];
			c_v3 += DATA_V3[l][t];
		}
	}
	c_i = c_i / R / mean_time;
	c_s = c_s / R / mean_time;
	c_u = c_u / R / mean_time;
	c_a = c_a / R / mean_time;
	c_v1 = c_v1 / R / mean_time;
	c_v2 = c_v2 / R / mean_time;
	c_v3 = c_v3 / R / mean_time;

	out << "method=mc lambda=" << lambda << " delta=" << delta << " gamma=" << gamma << " beta=" << beta << " sigma=" << sigma << " mu=" << mu << " m1=" << m1 << " m21=" << m21 << " m22=" << m22 << " m23=" << m23 << " p1=" << p1 << " p2=" << p2 << " p3=" << p3 << " v1=" << c_v1 << " v2=" << c_v2 << " v3=" << c_v3 << " i=" << c_i << " s=" << c_s << endl;
}

void run_mmca(TreeRoot *root, double lambda, double delta, double _gamma, double beta, double sigma, double mu, double m1, double m21, double m22, double m23, double p1, double p2, double p3, ostream &out)
{
	double DATA_US[R][T], DATA_AS[R][T], DATA_AV1[R][T], DATA_AV2[R][T], DATA_AV3[R][T], DATA_AI[R][T];
	int i, j, t, round;
	double q1 = p1, q2 = p1 * p2, q3 = p1 * p2 * p3;
	// 初始化
	for (t = 0; t < T; t++)
	{
		for (round = 0; round < R; round++)
		{
			DATA_US[round][t] = 0;
			DATA_AS[round][t] = 0;
			DATA_AV1[round][t] = 0;
			DATA_AV2[round][t] = 0;
			DATA_AV3[round][t] = 0;
			DATA_AI[round][t] = 0;
		}
	}
	double US[N_node], AS[N_node], AV1[N_node], AV2[N_node], AV3[N_node], AI[N_node], A[N_node];
	double r[N_node], q_as[N_node], q_us[N_node], q_av1[N_node], q_uv1[N_node], q_av2[N_node], q_uv2[N_node], q_av3[N_node], q_uv3[N_node];
	root->add_child("us", US)->add_child("us", r)->add_childr("us", m1)->add_childr("us", m21)->add_child("us", q_us);
	root->add_child("us", US)->add_child("us", r)->add_childr("us", m1)->add_childr("us", m21)->add_childr("ai", q_us);
	root->add_child("us", US)->add_child("us", r)->add_childr("us", m1)->add_child("uv1", m21)->add_child("av1", q_uv1);
	root->add_child("us", US)->add_child("us", r)->add_childr("us", m1)->add_child("uv1", m21)->add_childr("ai", q_uv1);
	root->add_child("us", US)->add_child("us", r)->add_child("as", m1)->add_childr("as", m21)->add_child("as", q_as);
	root->add_child("us", US)->add_child("us", r)->add_child("as", m1)->add_childr("as", m21)->add_childr("ai", q_as);
	root->add_child("us", US)->add_child("us", r)->add_child("as", m1)->add_child("av1", m21)->add_child("av1", q_av1);
	root->add_child("us", US)->add_child("us", r)->add_child("as", m1)->add_child("av1", m21)->add_childr("ai", q_av1);
	root->add_child("us", US)->add_childr("as", r)->add_childr("as", m21)->add_child("as", q_as);
	root->add_child("us", US)->add_childr("as", r)->add_childr("as", m21)->add_childr("ai", q_as);
	root->add_child("us", US)->add_childr("as", r)->add_child("av1", m21)->add_child("av1", q_av1);
	root->add_child("us", US)->add_childr("as", r)->add_child("av1", m21)->add_childr("ai", q_av1);

	root->add_child("as", AS)->add_child("us", delta)->add_childr("us", m1)->add_childr("us", m21)->add_child("us", q_us);
	root->add_child("as", AS)->add_child("us", delta)->add_childr("us", m1)->add_childr("us", m21)->add_childr("ai", q_us);
	root->add_child("as", AS)->add_child("us", delta)->add_childr("us", m1)->add_child("uv1", m21)->add_child("av1", q_uv1);
	root->add_child("as", AS)->add_child("us", delta)->add_childr("us", m1)->add_child("uv1", m21)->add_childr("ai", q_uv1);
	root->add_child("as", AS)->add_child("us", delta)->add_child("as", m1)->add_childr("as", m21)->add_child("as", q_as);
	root->add_child("as", AS)->add_child("us", delta)->add_child("as", m1)->add_childr("as", m21)->add_childr("ai", q_as);
	root->add_child("as", AS)->add_child("us", delta)->add_child("as", m1)->add_child("av1", m21)->add_child("av1", q_av1);
	root->add_child("as", AS)->add_child("us", delta)->add_child("as", m1)->add_child("av1", m21)->add_childr("ai", q_av1);
	root->add_child("as", AS)->add_childr("as", delta)->add_childr("as", m21)->add_child("as", q_as);
	root->add_child("as", AS)->add_childr("as", delta)->add_childr("as", m21)->add_childr("ai", q_as);
	root->add_child("as", AS)->add_childr("as", delta)->add_child("av1", m21)->add_child("av1", q_av1);
	root->add_child("as", AS)->add_childr("as", delta)->add_child("av1", m21)->add_childr("ai", q_av1);

	root->add_child("av1", AV1)->add_child("uv1", delta)->add_childr("uv1", m1)->add_childr("uv1", m22)->add_child("av1", q_uv1);
	root->add_child("av1", AV1)->add_child("uv1", delta)->add_childr("uv1", m1)->add_childr("uv1", m22)->add_childr("ai", q_uv1);
	root->add_child("av1", AV1)->add_child("uv1", delta)->add_childr("uv1", m1)->add_child("uv2", m22)->add_child("av2", q_uv2);
	root->add_child("av1", AV1)->add_child("uv1", delta)->add_childr("uv1", m1)->add_child("uv2", m22)->add_childr("ai", q_uv2);
	root->add_child("av1", AV1)->add_child("uv1", delta)->add_child("av1", m1)->add_childr("av1", m22)->add_child("av1", q_av1);
	root->add_child("av1", AV1)->add_child("uv1", delta)->add_child("av1", m1)->add_childr("av1", m22)->add_childr("ai", q_av1);
	root->add_child("av1", AV1)->add_child("uv1", delta)->add_child("av1", m1)->add_child("av2", m22)->add_child("av2", q_av2);
	root->add_child("av1", AV1)->add_child("uv1", delta)->add_child("av1", m1)->add_child("av2", m22)->add_childr("ai", q_av2);
	root->add_child("av1", AV1)->add_childr("av1", delta)->add_childr("av1", m22)->add_child("av1", q_av1);
	root->add_child("av1", AV1)->add_childr("av1", delta)->add_childr("av1", m22)->add_childr("ai", q_av1);
	root->add_child("av1", AV1)->add_childr("av1", delta)->add_child("av2", m22)->add_child("av2", q_av2);
	root->add_child("av1", AV1)->add_childr("av1", delta)->add_child("av2", m22)->add_childr("ai", q_av2);

	root->add_child("av2", AV2)->add_child("uv2", delta)->add_childr("uv2", m1)->add_childr("uv2", m23)->add_child("av2", q_uv2);
	root->add_child("av2", AV2)->add_child("uv2", delta)->add_childr("uv2", m1)->add_childr("uv2", m23)->add_childr("ai", q_uv2);
	root->add_child("av2", AV2)->add_child("uv2", delta)->add_childr("uv2", m1)->add_child("uv3", m23)->add_child("av3", q_uv3);
	root->add_child("av2", AV2)->add_child("uv2", delta)->add_childr("uv2", m1)->add_child("uv3", m23)->add_childr("ai", q_uv3);
	root->add_child("av2", AV2)->add_child("uv2", delta)->add_child("av2", m1)->add_childr("av2", m23)->add_child("av2", q_av2);
	root->add_child("av2", AV2)->add_child("uv2", delta)->add_child("av2", m1)->add_childr("av2", m23)->add_childr("ai", q_av2);
	root->add_child("av2", AV2)->add_child("uv2", delta)->add_child("av2", m1)->add_child("av3", m23)->add_child("av3", q_av3);
	root->add_child("av2", AV2)->add_child("uv2", delta)->add_child("av2", m1)->add_child("av3", m23)->add_childr("ai", q_av3);
	root->add_child("av2", AV2)->add_childr("av2", delta)->add_childr("av2", m23)->add_child("av2", q_av2);
	root->add_child("av2", AV2)->add_childr("av2", delta)->add_childr("av2", m23)->add_childr("ai", q_av2);
	root->add_child("av2", AV2)->add_childr("av2", delta)->add_child("av3", m23)->add_child("av3", q_av3);
	root->add_child("av2", AV2)->add_childr("av2", delta)->add_child("av3", m23)->add_childr("ai", q_av3);

	root->add_child("av3", AV3)->add_child("uv3", delta)->add_childr("uv3", m1)->add_child("av3", q_uv3);
	root->add_child("av3", AV3)->add_child("uv3", delta)->add_childr("uv3", m1)->add_childr("ai", q_uv3);
	root->add_child("av3", AV3)->add_child("uv3", delta)->add_child("av3", m1)->add_child("av3", q_av3);
	root->add_child("av3", AV3)->add_child("uv3", delta)->add_child("av3", m1)->add_childr("ai", q_av3);
	root->add_child("av3", AV3)->add_childr("av3", delta)->add_child("av3", q_av3);
	root->add_child("av3", AV3)->add_childr("av3", delta)->add_childr("ai", q_av3);

	root->add_child("ai", AI)->add_child("ui", delta)->add_child("ai", m1)->add_childr("ai", mu);
	root->add_child("ai", AI)->add_child("ui", delta)->add_child("ai", m1)->add_child("as", mu);
	root->add_child("ai", AI)->add_child("ui", delta)->add_childr("ui", m1)->add_childr("ai", mu);
	root->add_child("ai", AI)->add_child("ui", delta)->add_childr("ui", m1)->add_child("us", mu);
	root->add_child("ai", AI)->add_childr("ai", delta)->add_childr("ai", mu);
	root->add_child("ai", AI)->add_childr("ai", delta)->add_child("as", mu);

	for (round = 0; round < R; round++)
	{
		for (i = 0; i < N_node; i++)
		{ // 初始条件
			AS[i] = 0;
			AV1[i] = AV2[i] = AV3[i] = 0;
			AI[i] = rand() * 0.2 / RAND_MAX;
			US[i] = 1 - AS[i] - AI[i] - AV1[i] - AV2[i] - AV3[i];
		}
		for (t = 1; t < T; t++)
		{
			for (i = 0; i < N_node; i++)
			{
				A[i] = AI[i] + AV1[i] + AV2[i] + AV3[i] + AS[i];
			}
			for (i = 0; i < N_node; i++)
			{
				r[i] = q_as[i] = q_us[i] = q_av1[i] = q_uv1[i] = q_av2[i] = q_uv2[i] = q_av3[i] = q_uv3[i] = 1.0;
				for (j = 0; j < degree1[i]; j++)
				{
					q_as[i] *= (1 - AI[matrix1[i][j]] * _gamma * beta);
					q_us[i] *= (1 - AI[matrix1[i][j]] * beta);
					q_av1[i] *= (1 - AI[matrix1[i][j]] * _gamma * beta * q1);
					q_uv1[i] *= (1 - AI[matrix1[i][j]] * _gamma * beta * q1);
					q_av2[i] *= (1 - AI[matrix1[i][j]] * _gamma * beta * q2);
					q_uv2[i] *= (1 - AI[matrix1[i][j]] * _gamma * beta * q2);
					q_av3[i] *= (1 - AI[matrix1[i][j]] * _gamma * beta * q3);
					q_uv3[i] *= (1 - AI[matrix1[i][j]] * _gamma * beta * q3);
				}
				for (j = 0; j < degree2[i]; j++)
				{
					r[i] *= (1 - A[matrix2[i][j]] * lambda);
				}
				root->get_route_length(i);
				US[i] = root->get_weight("us");
				AI[i] = root->get_weight("ai");
				AS[i] = root->get_weight("as");
				AV1[i] = root->get_weight("av1");
				AV2[i] = root->get_weight("av2");
				AV3[i] = root->get_weight("av3");
				DATA_US[round][t] += US[i];
				DATA_AS[round][t] += AS[i];
				DATA_AI[round][t] += AI[i];
				DATA_AV1[round][t] += AV1[i];
				DATA_AV2[round][t] += AV2[i];
				DATA_AV3[round][t] += AV3[i];
			} // 节点循环
			DATA_US[round][t] /= N_node;
			DATA_AS[round][t] /= N_node;
			DATA_AI[round][t] /= N_node;
			DATA_AV1[round][t] /= N_node;
			DATA_AV2[round][t] /= N_node;
			DATA_AV3[round][t] /= N_node;
		} // 时间循环
	}	  // 随机网络循环

	double c_i, c_s, c_v1, c_v2, c_v3, c_u, c_a;
	int mean_time = 10;
	c_i = c_s = c_v1 = c_v2 = c_v3 = c_u = c_a = 0;
	for (t = T - mean_time; t < T; t++)
	{
		for (round = 0; round < R; round++)
		{
			c_s += DATA_US[round][t] + DATA_AS[round][t];
			c_i += DATA_AI[round][t];
			c_v1 += DATA_AV1[round][t];
			c_v2 += DATA_AV2[round][t];
			c_v3 += DATA_AV3[round][t];
			c_a += DATA_AI[round][t] + DATA_AV1[round][t] + DATA_AV2[round][t] + DATA_AV3[round][t] + DATA_AS[round][t];
		}
	} // shi jian xunhuan
	c_i = c_i / R / mean_time;
	c_s = c_s / R / mean_time;
	c_u = c_u / R / mean_time;
	c_a = c_a / R / mean_time;
	c_v1 = c_v1 / R / mean_time;
	c_v2 = c_v2 / R / mean_time;
	c_v3 = c_v3 / R / mean_time;
	out << "method=mc lambda=" << lambda << " delta=" << delta << " gamma=" << _gamma << " beta=" << beta << " sigma=" << sigma << " mu=" << mu << " m1=" << m1 << " m21=" << m21 << " m22=" << m22 << " m23=" << m23 << " p1=" << p1 << " p2=" << p2 << " p3=" << p3 << " v1=" << c_v1 << " v2=" << c_v2 << " v3=" << c_v3 << " i=" << c_i << " s=" << c_s << endl;
}