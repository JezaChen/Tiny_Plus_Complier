//
// Created by jeza1 on 2018/11/21.
//

#include <sstream>
#include <vector>
#include <iostream>
#include "globals.h"
#include "cgen.h"
#include "parser.h"

MidCodeVec midCodeVec;
AnalyzingStack analyzingStack;
TFList tfList;
bool has_error;
//std::string num_to_str(int num);


static int t_num;

/***
 * @brief 生成t前缀的符号名
 * @details 用于表示中间结果
 * **/
std::string get_t_num_str() {
    std::stringstream ss;
    ss << 't' << t_num;
    t_num++; //记得要++啊
    return ss.str();
}

static int label_num;

/***
 * @brief 生成Label前缀的符号名
 * @details 用于表示中间结果
 * **/
std::string get_label_num_str() {
    label_num++; //记得要++啊
    std::stringstream ss;
    ss << label_num;
    return ss.str();
}


/***
 * @brief 数字转字符串
 * */
std::string num_to_str(int num) {
    std::stringstream ss;
    ss << num;
    return ss.str();
}

/****************************************************/
//用于存储每个节点对应的label值，用于拉链回填能够快速回填
static std::map<TreeNode *, int> start_label;

void set_label_start_of(TreeNode *node, int label) {
    start_label[node] = label;
}

int get_label_start_of(TreeNode *node) {
    return start_label[node];
}

static std::map<TreeNode *, int> else_label;

void set_label_start_of_else_of(TreeNode *node, int label) {
    else_label[node] = label;
}

int get_label_start_of_else_of(TreeNode *node) {
    return else_label[node];
}

static std::map<TreeNode *, int> end_label;

void set_label_end_of(TreeNode *node, int label) {
    end_label[node] = label;
}

int get_label_end_of(TreeNode *node) {
    return end_label[node];
}

/*********************************************/

void traversal_back(TreeNode *root) {
    if (!root)
        return;
    TreeNode *c1 = root->child[0], *c2 = root->child[1], *c3 = root->child[2];
    //来一遍后续遍历

    /**********************************/
    /**REPEAT语句需要在执行语句前加LABEL**/
    /***********************************/
    if (root->nodeType == REPEAT_STMT) {
        emit(C_LABEL, get_label_num_str());
        set_label_start_of(root, label_num);
    }
    traversal_back(c1);

    /**********************************/
    /**IF语句需要在then, else语句加LABEL**/
    /**直接在父节点操作，如果深入到子节点，就没法确定是不是IF语句的孩子了**/
    /***********************************/
    if (root->nodeType == IF_STMT || root->nodeType == WHILE_STMT) {
        emit(C_LABEL, get_label_num_str());
        set_label_start_of(c2, label_num);
    }
    traversal_back(c2);

    if (root->nodeType == IF_STMT) {
        if (c3) {
            //如果else子句存在，需要在then字句后面加个goto，还要额外的回填操作
            //否则else子句会紧接then字句后执行，这是不可能的
            int nextlist_index = emit(C_GOTO, "XXX");
            tfList.makelist(NEXTLIST, root, nextlist_index);
        }

        emit(C_LABEL, get_label_num_str());
        set_label_start_of_else_of(root, label_num);
    } else if (root->nodeType == REPEAT_STMT) {
        //REPEAT语句需要在语句后再加个LABEL
        emit(C_LABEL, get_label_num_str());
        set_label_end_of(root, label_num);
    } else if (root->nodeType == WHILE_STMT) {
        //WHILE语句需要再执行语句紧接一个GOTO，指向判断前面
        int nextlist_index = emit(C_GOTO, "XXX");
        tfList.makelist(NEXTLIST, c2, nextlist_index); //c2或者root都可以，因为next都指向判断语句

        //WHILE语句需要在语句后再加个LABEL
        emit(C_LABEL, get_label_num_str());
        set_label_end_of(root, label_num);
    }


    traversal_back(c3);
    if (root->nodeType == IF_STMT ) {
        if (c3) {
            emit(C_LABEL, get_label_num_str()); //else执行后加一个label
            tfList.backpatch(NEXTLIST, root, label_num); //then的next指向该label
        }
    }

    std::string s2_str, s1_str, t_str, tmp_str;
    int true_index; //用于拉链回填，if xxx goto xxx 的序号
    TreeNode *child1, *child2, *child3; //用于拉链回填，and、or节点的孩子
    std::vector<int> merged_list; //用于表示拉链回填中的合并链表
    switch (root->nodeType) {
        case FACTOR:
            if (root->tk) {
                switch (root->tk->kind) {
                    case TK_TRUE:
                        analyzingStack.push(root, "true");
                        break;
                    case TK_FALSE:
                        analyzingStack.push(root, "false");
                        break;
                    case NUM:
                        analyzingStack.push(root, num_to_str(root->tk->i_val));
                        break;
                    case ID:
                        analyzingStack.push(root, root->tk->s_val);
                        break;
                    case STRING:
                        analyzingStack.push(root, "\'" + root->tk->s_val + "\'");
                        break;
                }
            } else { //tk为空的唯一一种情况是括号嵌套的结构
                /*s1_str = analyzingStack.pop().val;
                t_str = get_t_num_str();
                emit(C_ASSIGN, t_str, s1_str);
                analyzingStack.push(root, t_str);*/
                //fixed 应该不要做任何操作，因为嵌套的后一次都已经搞定好中间变量了，而且只有一个中间变量
            }
            break;
            /*****中间代码生成——加减乘除****/
            /***要从分析栈中弹出前两个元素***/
            /**加减乘除的结果作为中间结果保存**/
        case ADD_EXP:
            s2_str = analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val;
            t_str = get_t_num_str();
            emit(C_ADD, t_str, s1_str, s2_str);
            analyzingStack.push(root, t_str);
            break;
        case SUB_EXP:
            s2_str = analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val;
            t_str = get_t_num_str();
            emit(C_SUB, t_str, s1_str, s2_str);
            analyzingStack.push(root, t_str);
            break;
        case DIV_EXP:
            s2_str = analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val;
            t_str = get_t_num_str();
            emit(C_DIV, t_str, s1_str, s2_str);
            analyzingStack.push(root, t_str);
            break;
        case MUL_EXP:
            s2_str = analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val;
            t_str = get_t_num_str();
            emit(C_MUL, t_str, s1_str, s2_str);
            analyzingStack.push(root, t_str);
            break;
            /**********中间代码生成——赋值*********/
            /***赋值一般是中间结果，在栈中弹出一个***/
        case ASSIGN_STMT:
            s1_str = analyzingStack.pop().val;
            //t_str = get_t_num_str();
            t_str = root->tk->s_val; //要赋值的变量已经作为综合属性存在了节点当中
            emit(C_ASSIGN, t_str, s1_str);
            //analyzingStack.push(root, t_str);
            break;
            /****************中间代码生成——写****************/
            /***分析root的孩子，如果是factor，读factor的变量名（tk->sval属性）**/
            /**如果是表达式，则是中间变量，从栈中弹出一个**/
        case WRITE_STMT:
            if (root->child[0]->nodeType != FACTOR)
                t_str = analyzingStack.pop().val;
            else
                t_str = root->child[0]->tk->s_val;
            emit(C_WRITE, t_str);
            //没有中间结果，不用入栈
            break;
            /****************中间代码生成——读****************/
            /**根据语法规则，读的对象只能是id，因而从root的综合属性中提取出来即可**/
            /**在语法分析的时候已将读的对象的token'存进节点的tk对象中去**/
        case READ_STMT:
            t_str = root->tk->s_val;
            emit(C_READ, t_str);
            //没有中间结果，不用入栈
            break;
            /*************中间代码生成——大小关系判断**********/
            /****根据判断，判断的两者已经在栈中了，先提取出来****/
            /****************先输出Label语句***************/
            /********IF CONDITION GOTO XXX***************/
            /**********GOTO XXX**************************/
        case GEQ_EXP:
            t_str = get_label_num_str();
            emit(C_LABEL, t_str);
            set_label_start_of(root, label_num);

            tmp_str = ">=" + analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val + tmp_str;

            t_str = "XXX"; //还没开始拉链回填

            true_index = emit(C_IF, t_str, s1_str);
            emit(C_GOTO, "XXX");

            /*********拉链回填操作*********/
            tfList.makelist(TRUELIST, root, true_index);
            tfList.makelist(FALSELIST, root, true_index + 1);
            /****************************/
            //analyzingStack.push(root, s1_str); //投进栈中，用于父节点拉链回填操作
            break;
        case GTR_EXP:
            t_str = get_label_num_str();
            emit(C_LABEL, t_str);
            set_label_start_of(root, label_num);

            tmp_str = ">" + analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val + tmp_str;

            t_str = "XXX"; //还没开始拉链回填

            true_index = emit(C_IF, t_str, s1_str);
            emit(C_GOTO, "XXLEQX");
            /*********拉链回填操作*********/
            tfList.makelist(TRUELIST, root, true_index);
            tfList.makelist(FALSELIST, root, true_index + 1);
            /****************************/
            //analyzingStack.push(root, s1_str); //投进栈中，用于父节点拉链回填操作
            break;
        case LEQ_EXP:
            t_str = get_label_num_str();
            emit(C_LABEL, t_str);
            set_label_start_of(root, label_num);

            tmp_str = "<=" + analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val + tmp_str;

            t_str = "XXX"; //还没开始拉链回填

            true_index = emit(C_IF, t_str, s1_str);
            emit(C_GOTO, "XXX");
            /*********拉链回填操作*********/
            tfList.makelist(TRUELIST, root, true_index);
            tfList.makelist(FALSELIST, root, true_index + 1);
            /****************************/
            //analyzingStack.push(root, s1_str); //投进栈中，用于父节点拉链回填操作
            break;
        case LSS_EXP:
            t_str = get_label_num_str();
            emit(C_LABEL, t_str);
            set_label_start_of(root, label_num);

            tmp_str = "<" + analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val + tmp_str;

            t_str = "XXX"; //还没开始拉链回填

            true_index = emit(C_IF, t_str, s1_str);
            emit(C_GOTO, "XXX");
            /*********拉链回填操作*********/
            tfList.makelist(TRUELIST, root, true_index);
            tfList.makelist(FALSELIST, root, true_index + 1);
            /****************************/
            //analyzingStack.push(root, s1_str); //投进栈中，用于父节点拉链回填操作
            break;
        case EQU_EXP:
            t_str = get_label_num_str();
            emit(C_LABEL, t_str);
            set_label_end_of(root, label_num);

            tmp_str = "=" + analyzingStack.pop().val;
            s1_str = analyzingStack.pop().val + tmp_str;

            t_str = "XXX";

            true_index = emit(C_IF, t_str, s1_str);
            emit(C_GOTO, "XXX");
            /*********拉链回填操作*********/
            tfList.makelist(TRUELIST, root, true_index);
            tfList.makelist(FALSELIST, root, true_index + 1);
            /****************************/
            break;
        case LOG_AND_EXP:
            child1 = root->child[0];
            child2 = root->child[1];

            //fixed and节点的开始label应定位到第一个条件的label
            //多个and 遍历过程时从右到左
            set_label_start_of(root, get_label_start_of(child1));

            //置位child1的真链
            tfList.backpatch(TRUELIST, child1, get_label_start_of(child2));
            //合并假链
            merged_list = tfList.mergelist(FALSELIST, child1, child2);
            tfList.setlist(FALSELIST, root, merged_list);
            //赋予真链
            tfList.setlist(TRUELIST, root, tfList.getlist(TRUELIST, child2));
            break;
        case LOG_OR_EXP:
            child1 = root->child[0];
            child2 = root->child[1];

            set_label_start_of(root, get_label_start_of(child1));

            //置位child2的假链
            tfList.backpatch(FALSELIST, child1, get_label_start_of(child2));
            //合并真链
            merged_list = tfList.mergelist(TRUELIST, child1, child2);
            tfList.setlist(TRUELIST, root, merged_list);
            //赋予假链
            tfList.setlist(FALSELIST, root, tfList.getlist(FALSELIST, child2));
            break;
        case IF_STMT:
            child1 = root->child[0];
            child2 = root->child[1];
            child3 = root->child[2];

            //置位child1的真链
            tfList.backpatch(TRUELIST, child1, get_label_start_of(child2));
            //置位child1的假链
            tfList.backpatch(FALSELIST, child1, get_label_start_of_else_of(root));
            break;
        case REPEAT_STMT:
            child1 = root->child[0];
            child2 = root->child[1];

            //置位child2的假链到执行语句前
            tfList.backpatch(FALSELIST, child2, get_label_start_of(root));
            //置位child2的真链到判断语句后
            tfList.backpatch(TRUELIST, child2, get_label_end_of(root));
            break;
        case WHILE_STMT:
            child1 = root->child[0];
            child2 = root->child[1];

            tfList.backpatch(TRUELIST, child1, get_label_start_of(child1));
            tfList.backpatch(FALSELIST, child1, get_label_end_of(root));
            tfList.backpatch(NEXTLIST, child2, get_label_start_of(child1));
    }
}

int emit(CodeType type, const std::string &t, const std::string &s1, const std::string &s2) {
    return midCodeVec.push(type, s1, s2, t);
}

void printMidCodeVec() {

    //todo just for test
    for (int i = 0; i < midCodeVec.codeVec.size(); i++) {
        //std::cout << midCodeVec.get_item_str(i) << std::endl;
        fprintf(code, "%s\n", midCodeVec.get_item_str(i).c_str());
    }
}

void cgen() {
    //midCodeVec = MidCodeVec();
    // analyzingStack = AnalyzingStack();
    has_error = false;
    TreeNode *root = parse();
    if (has_error) {
        fprintf(code, "Compile Error...");
        return;
    }
    traversal_back(root);
    if (is_optimized)
        midCodeVec.shrink_codes();
    printMidCodeVec();
}