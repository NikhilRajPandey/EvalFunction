#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


typedef struct Node{
    float number;
    char sign;
    struct Node* next_node;
}Node;


float evaluate_two_term(Node *first_term,Node *second_term){
    switch (first_term->sign){
        case '+':return second_term->number+first_term->number;
        case '-':return second_term->number-first_term->number;
        case '*':return second_term->number*first_term->number;
        case '/':return second_term->number/first_term->number;
    }
}
int give_presidence(char sign){
    switch (sign){
        case '+':return 0;
        case '-':return 1;
        case '*':return 2;
        case '/':return 3;
    }
    return -1; // else
}
Node* empty_stack(Node *term,int next_sign){ // Note next_sign is the precedence of next_sign
    // Evaluate all the terms in the Node
    // My expression will be in increasing operator importance and the argument term will be the end ones.
    Node *next_term = term->next_node;

    while (term->next_node != NULL && give_presidence(term->sign) > next_sign){
        next_term->number = evaluate_two_term(term,next_term);
        free(term);
        term = next_term;
        next_term = next_term->next_node;
    }
    
    return term;
}
void print_stack(Node *start){
    if(start->next_node == start){
        printf("Founded The\n");
        return;
    }
    while (start != NULL){
        printf("\n|Number %f %c|\n",start->number,start->sign);
        start = start->next_node;
    }
    
}
float evaluate_expression(char expression[]){
    // I am assuming here that the expression is correct
    static int index = 0;
    Node *terms_ = malloc(sizeof(Node));
    terms_->number = 0;
    // if a+b then [a,+],[b,\0]
    
    while (expression[index] != '\0'){
        if(expression[index] != ' '){
            if (expression[index] == '('){
                index++;
                printf("I:%d\n",index);
                terms_->number=evaluate_expression(expression);
                printf("I:%d\n",index);
                printf("e%f\n",terms_->number);
                print_stack(terms_);
                printf("Hereend\n");
            }
            else if(expression[index] == ')'){
                // I am not breaking because index++ is important
                terms_ = empty_stack(terms_,-1);
                // printf("printing: \n");
                // print_stack(terms_);
                // printf("*******g: \n");
                float returning_ = terms_->number;
                free(terms_);
                return returning_;
            }
            else if(isdigit(expression[index])){
                terms_->number = terms_->number*10 + (expression[index]-'0');
                // Suppose '45' a str then 4*10+5
                // (expression[index]+'0') will convert it into int
                //https://stackoverflow.com/questions/439573/how-to-convert-a-single-char-into-an-int
            }
            else{ // I Founded a operator
            // printf("stack_printing\n");
                if(give_presidence(expression[index]) < give_presidence(terms_->sign)){
                    terms_ = empty_stack(terms_,give_presidence(expression[index]));
                }
                Node *new_term = malloc(sizeof(Node));
                new_term->number = 0;
                new_term->sign = expression[index];
                new_term->next_node = terms_;
                terms_ = new_term;

            }
        }
        index++;
    }
    printf("out\n");
    print_stack(terms_);
    index=0;// Reseting index_ because its a static variable
    terms_ = empty_stack(terms_,-1);
    // Used -1 so that loop runs completly in that function
    float returning_ = terms_->number;
    free(terms_);
    return returning_;
}

int main(){
    // Node * test1 = malloc(sizeof(Node));
    // test1->number = 4;
    // test1->sign = '-';
    // test1->next_node = malloc(sizeof(Node));
    // test1->next_node->number = 6;
    // test1->next_node->sign = '+';
    // test1->next_node->next_node = malloc(sizeof(Node));
    // test1->next_node->next_node->number = 20;
    // test1->next_node->next_node->sign = '\0';
    // test1->next_node->next_node->next_node = NULL;
    // empty_stack(test1);
    // printf("Value %f\n",test1->number);
    // if(test1->next_node == NULL){
    //     printf("yes\n");
    // }
    char testing_[] = "(4*6+5-5/5+458+42-14*(4+56+6)+9+41)/2";
    float returning_ = evaluate_expression(testing_);
    printf("This: %f \n",returning_);
    return 0;
}
