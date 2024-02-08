#include <iostream>
#include <ctime>
#include <cmath>
using namespace std;
class ParamList
{
private:
    char *str;
    double *starts;
    double *stops;
    double *steps;
    int *current_steps;
    int total_count;
    int current;
    clock_t time_rec;

public:
    string *names;
    int length;
    const static int NOT_VALID = -999.0;
    ParamList(int max)
    {
        this->total_count = -1;
        this->length = 0;
        this->current = 0;
        this->time_rec = -1;
        this->names = new string[max];
        this->starts = new double[max];
        this->stops = new double[max];
        this->steps = new double[max];
        this->current_steps = new int[max];
        this->str = new char[1024];
    }
    ~ParamList()
    {
        delete this->names;
        delete this->starts;
        delete this->stops;
        delete this->steps;
        delete this->current_steps;
        delete this->str;
    }
    void header(FILE *pf)
    {
        fprintf(pf, "-----------------------------------------[params]---------------------------------------\n");
        for (int i = 0; i < this->length; i++)
        {
            if (this->starts[i] + this->steps[i] <= this->stops[i])
            {
                fprintf(pf, "param:%s start:%f stop:%f step:%f\n", this->names[i].c_str(), this->starts[i], this->stops[i], this->steps[i]);
            }
            else
            {
                fprintf(pf, "param:%s value:%f\n", this->names[i].c_str(), this->starts[i]);
            }
        }
        fprintf(pf, "-----------------------------------------[datas]-----------------------------------------\n");
    }
    void setParam(string name, double start, double stop, double step)
    {
        int i;
        for (i = 0; i < this->length; i++)
        {
            if (name == this->names[i])
                break;
        }
        this->names[i] = name;
        this->starts[i] = start;
        this->stops[i] = stop + step * 0.1;
        this->steps[i] = step;
        this->current_steps[i] = 0;
        this->length++;
    }
    void setParam(string name, double value)
    {
        this->setParam(name, value, value * 1.1, value + 100);
    }
    double getParam(string name)
    {
        int i;
        for (i = 0; i < this->length; i++)
        {
            if (name == this->names[i])
                return this->getParam(i);
        }
        return NOT_VALID;
    }
    const char *desc()
    {
        char target[1024];
        int total_len = 0, len, cursor;
        for (int i = 0; i < this->length; i++)
        {
            double v = this->getParam(i);
            len = sprintf(target, "%s=%f ", this->names[i].c_str(), v);
            cursor = 0;
            do
            {
                str[total_len + cursor] = target[cursor];
                cursor++;
            } while (cursor < len);
            total_len += len;
        }
        str[total_len] = '\0';
        return this->str;
    }
    int total_steps()
    {
        if (this->total_count > 0)
        {
            return this->total_count;
        }
        int groups = 1;
        for (int i = 0; i < this->length; i++)
            groups *= (int)((this->stops[i] - this->starts[i]) / this->steps[i] + 1);
        this->total_count = groups;
        return groups;
    }
    double operator[](string name)
    {
        return this->getParam(name);
    }
    double getParam(int i)
    {
        double value = this->starts[i] + this->steps[i] * this->current_steps[i];
        if ((value - this->stops[i])*((int)(this->stops[i] > 0) - (int)(this->stops[i] < 0))>0)
        {
            return NOT_VALID;
        }
        else
        {
            return value;
        }
    }
    bool next()
    {
        this->current_steps[0]++;
        bool valid;
        int i;
        int record = 0;
        do
        {
            valid = true;
            for (i = record; i < this->length; i++)
            {
                if (this->getParam(i) == NOT_VALID)
                {
                    if (i == this->length - 1)
                    {
                        return false;
                    }
                    valid = false;
                    int j = i;
                    this->current_steps[i + 1]++;
                    while (j >= record)
                    {
                        this->current_steps[j--] = 0;
                    }
                    record = i;
                    break;
                }
            }
        } while (!valid);
        return true;
    }
    void show_progress()
    {
        clock_t now = clock();
        double time_cost = (double)(now - this->time_rec) / CLOCKS_PER_SEC;
        this->time_rec = now;
        int all_num = 50;
        double percentage = 100.0 * this->current / this->total_steps();
        double rest_time = time_cost * (this->total_steps() - this->current);
        if (this->current == 0)
        {
            rest_time = 999;
        }
        int now_num = (int)(all_num * percentage / 100);
        printf("\rfinish[%d/%d||%.2lf%%]:", this->current, this->total_steps(), percentage);
        for (int i = 0; i < now_num; i++)
        {
            printf("=");
        }
        printf(">");
        for (int i = now_num; i < all_num; i++)
        {
            printf(" ");
        }
        printf("|rest time:%.2fs.", rest_time);
        if (this->current >= this->total_steps())
        {
            printf("\n");
        }
        fflush(stdout);
        this->current++;
    }
};
