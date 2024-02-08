#include <stdio.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char const *argv[])
{
    const char *param_name;
    double start, stop, step;
    /* code */
    cout << argc;
    for (int i = 1; i < argc; i++)
    {
        cout << argv[i] << endl;
        if (strcmp(argv[i], "123") == 0)
        {
            cout << "check2 123" << endl;
        }
        else
        {
            param_name = argv[i];
            sscanf(argv[i + 1], "%lf", &start);
            try
            {
                sscanf(argv[i + 2], "%lf", &stop);
                sscanf(argv[i + 3], "%lf", &step);
                cout << "param:" << param_name << " start:" << start << " stop:" << stop << " step:" << step << endl;
                i += 2;
                continue;
            }
            catch (exception e)
            {
                cout << "param:" << param_name << " value:" << start << endl;
                i++;
                continue;
            }
        }
    }
    getchar();
    return 0;
}
