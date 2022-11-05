#include <stdio.h>
#include <string.h>
#include "unity.h"
#include "jacobi.h"

long double EPSILON;

void setUp(void){
    EPSILON = 0.001;
}

void tearDown(void){}

int different(long double a, long double b){
    char a_str[128];
    char b_str[128];
    char * one;
    char * two;

    sprintf(a_str,"%Lf",a);
    sprintf(b_str,"%Lf",a);

    if (strlen(a_str) > strlen(b_str)){
        one = b_str;
        two = a_str;
    }
    else{
        one = a_str;
        two = b_str;
    }

    for (int i = 0; i < strlen(one); i++){
        int d1, d2;
        d1 = (int)one[i] - 48;
        d2 = (int)two[i] - 48;
        if (d1 != d2)
            return 1;
    }
    return 0;
}

int check_results(linear_equation_system lin_sys){
    for (int i = 0; i < lin_sys.rows; i++) {
        long double sum = 0.0;
        for (int j = 0; j < lin_sys.cols; j++) {
            sum += lin_sys.a[i * lin_sys.cols + j] * lin_sys.x[j];
        }
        if (fabsl(sum - lin_sys.b[i]) > EPSILON)
            return 0;
    }
    return 1;
}

void test_jacobi(void) {
    FILE * file;
    linear_equation_system lin_sys;
    file = fopen("examples/test.csv","r");

    TEST_ASSERT_TRUE(load_from_file(&lin_sys,file) != NULL);

    fclose(file);

    jacobi(&lin_sys,20);

    TEST_ASSERT_TRUE(check_results(lin_sys));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_jacobi);
    return UNITY_END();
}
