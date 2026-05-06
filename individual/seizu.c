#include <stdio.h>
#include <math.h>

// マクロ定義（セミコロンは不要。M_PIがmath.hにある場合はそれを使ってもOK）
#define PAI 3.1415926535897932
#define E_VAL 2.1e4  // Eは変数名と被りやすいのでE_VAL等にするのが安全
#define S_VAL 10.0   // 10,0 ではなく 10.0

// ==========================================
// 補助関数群
// ==========================================

// 1/2 は必ず 0.5 または 1.0/2.0 とする
double R_2(double R_1, double R_3) {
    return 0.5 * (R_1 + R_3);
}

double I_1(double B_1, double B_2, double R_0, double R_1, double D) {
    return (1.0 / 12.0) * (B_1 * pow(R_1 - R_0, 3) - (B_1 - B_2) * pow(R_1 - R_0 - D, 3));
}

double I_2(double B_1, double B_2, double R_0, double R_1, double R_3, double D) {
    // I_2にもDが関わるため引数に追加し、R_2の計算式を適用
    double r2 = R_2(R_1, R_3);
    return (1.0 / 12.0) * (B_1 * pow(r2 - R_0, 3) - (B_1 - B_2) * pow(r2 - R_0 - D, 3));
}

double I_3(double B_1, double B_2, double R_0, double R_3, double D) {
    return (1.0 / 12.0) * (B_1 * pow(R_3 - R_0, 3) - (B_1 - B_2) * pow(R_3 - R_0 - D, 3));
}

double p_1(double R_0, double R_1) {
    return 0.5 * (R_0 + R_1);
}

double p_2(double R_0, double R_1, double R_3) {
    return 0.5 * (R_0 + R_2(R_1, R_3));
}

double p_3(double R_0, double R_3) {
    return 0.5 * (R_0 + R_3);
}

// ==========================================
// メインのたわみ計算
// ==========================================
double delta(double R_0, double R_1, double R_3, double B_1, double B_2, double D) {
    // 数式が長すぎる場合は、項(term)ごとに分割するとバグを防げます
    double rho1 = p_1(R_0, R_1);
    double rho2 = p_2(R_0, R_1, R_3);
    double rho3 = p_3(R_0, R_3);

    double i1 = I_1(B_1, B_2, R_0, R_1, D);
    double i2 = I_2(B_1, B_2, R_0, R_1, R_3, D);
    double i3 = I_3(B_1, B_2, R_0, R_3, D);

    // 第1項
    double term1 = (1.0 / i1) * (0.25 * (PAI / 3.0 - sqrt(3.0) / 2.0) * pow(rho1, 3) 
                               + (2.0 - sqrt(3.0)) * S_VAL * pow(rho1, 2) 
                               + (PAI / 6.0) * pow(S_VAL, 2) * rho1);
    // 第2項
    double term2 = (1.0 / i2) * ((PAI / 12.0) * pow(rho2, 3) 
                               + (sqrt(3.0) - 1.0) * S_VAL * pow(rho2, 2) 
                               + (PAI / 6.0) * pow(S_VAL, 2) * rho2);
    // 第3項
    double term3 = (1.0 / i3) * (0.25 * (PAI / 3.0 + sqrt(3.0) / 2.0) * pow(rho3, 3) 
                               + S_VAL * pow(rho3, 2) 
                               + (PAI / 6.0) * pow(S_VAL, 2) * rho3);

    return (2.0 / E_VAL) * (term1 + term2 + term3);

}

// ==========================================
// 実行
// ==========================================
int main() {
    // ひとまずテスト用の適当な値を入れています（ゼロ割りを防ぐため）
    double R_0 = 150.0*0.5+5;
    double R_1[6] ={R_0+15,R_0+20,R_0+25,R_0+20,R_0+15,R_0+25} ;
    double B_2[6] = {4,6,7,4,6,7};
    double D[6] = {8,10,12,14,12,14};
    double B_1[6] = {12,28,21,14,16,20};
    double R_3[6] ={96.0,101.0,106.0,101.0,96.0,106.0}; 
    double e = 0.0; 
for (int i = 0; i < 6; i++) {
    double result = delta(R_0, R_1[i], R_3[i], B_1[i], B_2[i], D[i]);
    double e= 0.5 * (R_3[i] - sqrt(2*pow(R_1[i], 2) - pow(R_3[i],2)));
    printf("e: %f, たわみ量(delta): %f\n", e, result);
}
    return 0;
}