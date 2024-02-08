#include <stdio.h>
#include <fstream>
#include <time.h>
#include <string.h>
#include "TreeNodeV4.h"
#include "mc_mmca.cpp"
using namespace std;

int main(int argc, char const *argv[])
{
    bool mc_enable = false, mmca_enable = false;
    const char *param_name;
    char save_file_name[100] = "mmca_threhold_m23_p_3(1).txt";
    double start, stop, step;
    // 设置默认参数
    ParamList params(15);
    params.setParam("beta", 0.5, 1, 0.05);
    params.setParam("sigma", 1);
    params.setParam("m1", 0.5);
    params.setParam("m21", 0.5);
    params.setParam("m22", 0.5);
    params.setParam("m23", 0, 0.1, 0.01);
    params.setParam("p1", 0.5);
    params.setParam("p2", 0.5);
    params.setParam("p3", 0.3);
    params.setParam("lambda", 0.3);
    params.setParam("delta", 0.6);
    params.setParam("mu", 0.4);
    params.setParam("gamma", 0);
    // 从控制台读取参数
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "mc") == 0)
        {
            cout << "run mc" << endl;
            mc_enable = true;
        }
        else if (strcmp(argv[i], "mmca") == 0)
        {
            cout << "run mmca" << endl;
            mmca_enable = true;
        }
        else if (argv[i][0] == '-' && argv[i][1] == 'n' && argv[i][2] == '=')
        {
            int j = 3;
            do
            {
                save_file_name[j - 3] = argv[i][j];
                j++;
            } while (argv[i][j] != '\0');
            save_file_name[j - 3] = '\0';
            cout << "change save file:" << save_file_name << endl;
        }
        else
        {
            param_name = argv[i];
            sscanf(argv[i + 1], "%lf", &start);
            if (i == argc - 2)
            {
                cout << "set param:" << param_name << " value:" << start << endl;
                params.setParam(param_name, start);
                break;
            }
            else
            {
                int temp = argv[i + 2][0];
                if (temp >= 48 && temp <= 57)
                {
                    sscanf(argv[i + 2], "%lf", &stop);
                    sscanf(argv[i + 3], "%lf", &step);
                    cout << "set param:" << param_name << " start:" << start << " stop:" << stop << " step:" << step << endl;
                    params.setParam(param_name, start, stop, step);
                    i += 3;
                }
                else
                {
                    cout << "set param:" << param_name << " value:" << start << endl;
                    params.setParam(param_name, start);
                    i += 1;
                }
            }
        }
    }
    // 默认至少跑一个mc
    if (!(mc_enable || mmca_enable))
    {
        mc_enable = true;
    }
    ofstream pf;
    stringstream filename;
    pf.open(filename.str().c_str()); // 定义一个存储数据的文件
    save_matrix1("gamma=2.5-kmin=2-N=1000.xls");
    save_matrix2("xinxiceng_1000.xls");
    params.header(pf);
    double out_i = 0, beta_v = 0;
    if (mmca_enable)
    {
        double lambda, delta, gamma, beta, sigma, mu, m1, m21, m22, m23, p1, p2, p3;
	// 参数绑定
	lambda = param["lambda"], delta = param["delta"], gamma = param["gamma"], beta = param["beta"], sigma = param["sigma"], mu = param["mu"], m1 = param["m1"], m21 = param["m21"], m22 = param["m22"], m23 = param["m23"], p1 = param["p1"], p2 = param["p2"], p3 = param["p3"];
	
        TreeRoot *root = new TreeRoot();
        out_i = run_mmca(root, params, pf); // 进行计算
        delete root;
    }
    if (mc_enable)
        run_mc(params, pf); // 进行计算
    pf.close();
    return 0;
}