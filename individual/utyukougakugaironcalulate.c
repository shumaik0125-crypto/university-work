#include <stdio.h>
#include <math.h>
double calulate(double a) {
    double result;
    result =  2000*9.8*log(4000/(4000-6.0*pow(10,-4)*a));
    return result;
}
double calulate2(double b){
    double result2;
    result2 = 4000-6.0*pow(10,-4)*b;
    return result2;
}
double calulate3(double isp, double dv) {
    double epsilon = 0.1; // 構造比
    // (isp * 9.8) をカッコで括って、ひとまとめに割るのがポイント！
    double result3 = (exp(-dv / (isp * 9.8)) - epsilon) / (1 - epsilon);
    return result3;
}

int main() {
    double isp;
    // ispを1000から始めて10000まで1000刻みで計算
    for (isp = 1000; isp <= 10000; isp += 1000) {
        printf("%f\n", calulate3(isp, 8200.0));
    }
    return 0;
}