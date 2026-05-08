#include <stdio.h>
#include <math.h>

#define u_m 0.2
#define d 0.5
#define D_e 7.675
#define D_R 6.2
#define u_R 0.5
#define Pai 3.14159265358979323846
#define G 42000
#define n_z 2
#define q 20 //爪歯数のこと

//ここで、T_maxはモーメントなので単位はm・Nである。
double T_max(double P_max,double p_m,double b){
    return D_e/2.0*P_max*tan(p_m+b);
}

//ここでラチェットが滑り出すときの押し付け力P_Rを求める
// 【修正箇所】変数 p_R をスコープ内で参照できるよう引数に追加しました
double P_R(double P_max,double p_m,double b,double theta, double p_R){
    return tan(p_m+b)/tan(p_R+theta)*D_e/D_R*P_max;
}

//1巻きあたりのたわみ
double delta(double P_max){
    return 8*P_max*pow(D_R,3)/G/pow(d,4);
}

double calc_tooth_height(int q_val,double p,double theta) {
    // 3. 高さ h の計算 (h = p * tan(θ))
    double h = p * tan(theta);
    
    return h;
}

//全たわみの導出
// 【修正箇所】P_R関数の変更に伴い、引数に p_R を追加しました
double delta_s(double n,double P_max,double p_m,double b,double theta, double p_R) {
    // 分子: 8 * P_R * n * D_R^3
    double numerator = 8.0 * P_R(P_max,p_m,b,theta,p_R) * n * (D_R * D_R * D_R);
    
    // 分母: G * d^4
    double denominator = G * (d * d * d * d);
    
    return numerator / denominator;
}

int main(){
    double p_m=atan(u_m);
    double b=atan(d/Pai/D_e);
    double theta= 30*(Pai/180.0);
    double P_max = 8;
    double p_R = atan(u_R);
    double p = (Pai * D_R) / q; //ピッチのこと
    int n= 4.0; //有効巻き数
    int n_t= n+2*n_z; //総巻き数のこと
    double H_c= n_t*d; //ばねの密着高さ
    
    // ----------------------------------------------------
    // 追加で計算を依頼された項目のコード
    // ----------------------------------------------------

    // 計算に必要になる値を先に出しておく
    double h_tooth = calc_tooth_height(q, p, theta);
    double P_R_val = P_R(P_max, p_m, b, theta, p_R);
    double delta_s_val = delta_s(n, P_max, p_m, b, theta, p_R);

    // 1. 最小自由長 H_min
    // = 密着高さ + ラチェット滑り出し時のたわみ + 爪歯が乗り上げるための余裕(h)
    double H_min = H_c + delta_s_val + h_tooth;

    // 2. 自由長 H_s (> H_min)
    // = H_min より大きい適切な値（ここでは安全を考慮して切り上げ処理をしています）
    double H_s = ceil(H_min);

    // 3. ばね定数 k
    // k = (G * d^4) / (8 * n * D_R^3)
    double k = (G * pow(d, 4)) / (8.0 * n * pow(D_R, 3));

    // 4. 指定高さ（最大測定力作用時高さ ＝ 取付け高さ）
    // = 自由長から、滑り出し時のたわみ分を引いた高さ
    double H_install = H_s - delta_s_val;

    // 5. 最大圧縮時（爪歯空転時）の高さと荷重
    // 爪歯を乗り越えるため、取付け高さからさらに爪歯の高さ(h)分圧縮される
    double H_compress = H_install - h_tooth;
    double P_max_compress = P_R_val + (k * h_tooth);

    // ----------------------------------------------------
    // ここから下が、すべての関数と変数の出力処理です
    // ----------------------------------------------------
    printf("========== 関数の計算結果 ==========\n");
    printf("T_max             : %.5f\n", T_max(P_max, p_m, b));
    printf("P_R               : %.5f\n", P_R(P_max, p_m, b, theta, p_R));
    printf("delta             : %.5f\n", delta(P_max));
    printf("calc_tooth_height : %.5f\n", calc_tooth_height(q, p, theta));
    printf("delta_s           : %.5f\n", delta_s(n, P_max, p_m, b, theta, p_R));

    printf("\n========== 変数の値 ==========\n");
    printf("p_m               : %.5f\n", p_m);
    printf("b                 : %.5f\n", b);
    printf("theta             : %.5f\n", theta);
    printf("P_max             : %.5f\n", P_max);
    printf("p_R               : %.5f\n", p_R);
    printf("p                 : %.5f\n", p);
    printf("n                 : %d\n", n);
    printf("n_t               : %d\n", n_t);
    printf("H_c               : %.5f\n", H_c);
    printf("h_tooth           : %.5f\n", h_tooth);
    printf("P_R_val           : %.5f\n", P_R_val);
    printf("delta_s_val       : %.5f\n", delta_s_val);
    printf("H_min             : %.5f\n", H_min);
    printf("H_s               : %.5f\n", H_s);
    printf("k                 : %.5f\n", k);
    printf("H_install         : %.5f\n", H_install);
    printf("H_compress        : %.5f\n", H_compress);
    printf("P_max_compress    : %.5f\n", P_max_compress);

    return 0;
}