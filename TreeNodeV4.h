#ifndef _TREE_NODE_
#define _TREE_NODE_
#include <stdio.h>
#include <string.h>
#define MAX_STATE 8
using namespace std;
class Result
{
public:
    int result_num;
    const char *states[MAX_STATE];
    double weights[MAX_STATE];
    void Add(const char *state, double weight);
};
Result temp_result;
class TreeNode
{
protected:
    TreeNode(const char *state, double weight);
    TreeNode(const char *state, double *weight);

public:
    TreeNode()
    {
        children_num = 0;
        weight = 1;
        reverse = false;
        weight_src = NULL;
    }
    const char *state;                   // 节点状态
    TreeNode *children[MAX_STATE]; // 节点的父节点
    int children_num;
    double weight; // 节点到父节点的路径权重，如果节点是根节点，则表示该节点的概率
    bool reverse;
    double *weight_src; // 节点到父节点的路径权重，如果节点是根节点，则表示该节点的概率
    TreeNode *add_child(const char *state, double weight);
    TreeNode *add_child(const char *state, double *weight_src);
    TreeNode *add_childr(const char *state, double *weight_src);
    TreeNode *add_childr(const char *state, double weight_src);
    bool check(int index);
    double get_weight(int index);
    ~TreeNode()
    {
        if (this->weight_src != NULL)
            delete this->weight_src;
    }
};
class TreeRoot : public TreeNode
{
    bool checked;

public:
    TreeRoot()
    {
        checked = false;
    };
    double get_weight(const char *state);
    Result *get_route_length(int index);
    ~TreeRoot()
    {
    }
};

double TreeRoot::get_weight(const char *state)
{
    for (int i = 0; i < temp_result.result_num; i++)
    {
        if (state == temp_result.states[i])
        {
            return temp_result.weights[i];
        }
    }
    return 0;
};
void Result::Add(const char *state, double weight)
{
    for (int i = 0; i < this->result_num; i++)
    {
        if (this->states[i] == state)
        {
            this->weights[i] += weight;
            return;
        }
    }
    this->states[result_num] = state;
    this->weights[result_num++] = weight;
}
void cal_route_length(TreeNode *node, int index, double acc_w)
{
    acc_w *= node->get_weight(index);
    if (node->children_num == 0)
    {
        temp_result.Add(node->state, acc_w);
        return;
    }
    else
    {
        for (int i = 0; i < node->children_num; i++)
        {
            cal_route_length(node->children[i], index, acc_w);
        }
    }
}

TreeNode::TreeNode(const char *state, double weight)
{
    children_num = 0;
    reverse = false;
    weight_src = NULL;
    this->state = state;
    this->weight = weight;
}
TreeNode::TreeNode(const char *state, double *weight)
{
    children_num = 0;
    reverse = false;
    this->state = state;
    this->weight_src = weight;
}
TreeNode *TreeNode::add_child(const char *state, double *weight_src)
{
    for (int i = 0; i < this->children_num; i++)
    {
        if (this->children[i]->state == state)
        {
            return this->children[i];
        }
    }
    TreeNode *child = new TreeNode(state, weight_src);
    this->children[this->children_num++] = child;
    return child;
}
TreeNode *TreeNode::add_child(const char *state, double weight)
{
    for (int i = 0; i < this->children_num; i++)
    {
        if (this->children[i]->state == state)
        {
            this->children[i]->weight = weight;
            return this->children[i];
        }
    }
    TreeNode *child = new TreeNode(state, weight);
    this->children[this->children_num++] = child;
    return child;
}
TreeNode *TreeNode::add_childr(const char *state, double *weight_src)
{
    TreeNode *child = this->add_child(state, weight_src);
    child->reverse = true;
    return child;
}
TreeNode *TreeNode::add_childr(const char *state, double weight)
{
    TreeNode *child = this->add_child(state, weight);
    child->reverse = true;
    return child;
}
Result *TreeRoot::get_route_length(int index)
{
    if (this->checked)
    {
        this->check(index);
        this->checked = true;
    }
    temp_result.result_num = 0;
    cal_route_length(this, index, 1);
    return &temp_result;
}
bool TreeNode::check(int index)
{
    if (this->children_num == 0)
    {
        return true;
    }

    double sum = 0;
    for (int i = 0; i < this->children_num; i++)
    {
        sum += this->children[i]->get_weight(index);
        if (!this->children[i]->check(index))
        {
            printf("parent:%s,children:", this->state);
            for (int i = 0; i < this->children_num; i++)
            {
                printf("%s,", this->children[i]->state);
            }
            return false;
        }
    }
    if (sum < 0.99)
    {
        printf("parent:%s,children:", this->state);
        for (int i = 0; i < this->children_num; i++)
        {
            printf("%s,", this->children[i]->state);
        }
        return false;
    }
    return true;
}
double TreeNode::get_weight(int index)
{
    double weight;
    if (this->weight_src != NULL)
    {
        weight = this->weight_src[index];
    }
    else
    {
        weight = this->weight;
    }
    if (this->reverse)
    {
        return 1 - weight;
    }
    else
    {
        return weight;
    }
}
#endif
