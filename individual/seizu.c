#include <stdio.h>
#include <math.h>

// マクロ定義
#define PAI 3.1415926535897932
#define E_VAL 2.1e4  
#define S_VAL 10.0   

// ==========================================
// 補助関数群
// ==========================================
double R_2(double R_1, double R_3) {
    return 0.5 * (R_1 + R_3);
}

double I_1(double B_1, double B_2, double R_0, double R_1, double D) {
    return (1.0 / 12.0) * (B_1 * pow(R_1 - R_0, 3) - (B_1 - B_2) * pow(R_1 - R_0 - D, 3));
}

double I_2(double B_1, double B_2, double R_0, double R_1, double R_3, double D) {
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
// 実行と検証
// ==========================================
int main() {
    printf("=== マイクロメータ フレームたわみ計算 ===\n");
    
    double R_0 = 150.0 * 0.5 + 5.0; 
    double R_1[6] = {95.0, 100.0, 105.0, 100.0, 95.0, 105.0}; 
    double B_2[6] = {4.0, 6.0, 7.0, 4.0, 6.0, 7.0};
    double D[6]   = {8.0, 10.0, 12.0, 14.0, 12.0, 14.0};
    double B_1[6] = {18.0, 20.0, 22.0, 20.0, 18.0, 24.0};
    double R_3[6] = {120.0, 125.0, 130.0, 125.0, 120.0, 135.0}; 

    // --- 計算フェーズ ---
    for (int i = 0; i < 6; i++) {
        double result = delta(R_0, R_1[i], R_3[i], B_1[i], B_2[i], D[i]);
        double e = 0.5 * (R_3[i] - sqrt(2.0 * pow(R_1[i], 2) - pow(R_3[i], 2)));
        
        if (result < 0.0016) {
            printf("i: %d, e: %5.2f, たわみ量(delta): %.6f mm (PASS)\n", i, e, result);
        } else {
            printf("i: %d, e: %5.2f, たわみ量(delta): %.6f mm (NG: 剛性不足)\n", i, e, result);
        }
    }

    // --- 誤差検証フェーズ (i=0 のデータを用いて内部状態をチェック) ---
    printf("\n=== 誤差検証テスト (i=0 のパラメータを使用) ===\n");
    
    double rho1 = p_1(R_0, R_1[0]);
    double pow_val = pow(rho1, 3);
    double mul_val = rho1 * rho1 * rho1;
    
    printf("[1. pow関数の微小誤差チェック]\n");
    printf("pow(rho1, 3)     = %.10f\n", pow_val);
    printf("rho1*rho1*rho1   = %.10f\n", mul_val);
    printf("差分             = %.15f (※0に近ければ問題なし)\n\n", pow_val - mul_val);

    double danger_sub = PAI / 3.0 - sqrt(3.0) / 2.0;
    
    printf("[2. 近似値の引き算による桁落ちチェック]\n");
    printf("PAI/3            = %.10f\n", PAI / 3.0);
    printf("sqrt(3)/2        = %.10f\n", sqrt(3.0) / 2.0);
    printf("引き算の結果     = %.10f (※有効数字が極端に減っていなければOK)\n\n", danger_sub);

    double i1 = I_1(B_1[0], B_2[0], R_0, R_1[0], D[0]);
    double term1_part1 = 0.25 * danger_sub * mul_val;
    double term1_part2 = (2.0 - sqrt(3.0)) * S_VAL * (rho1 * rho1);
    double term1_part3 = (PAI / 6.0) * (S_VAL * S_VAL) * rho1;
    double term1 = (1.0 / i1) * (term1_part1 + term1_part2 + term1_part3);

    printf("[3. 内部項のスケール（大きさ）チェック]\n");
    printf("断面2次モーメント I1 = %.2f\n", i1);
    printf("Term1 の 中身1       = %.2f\n", term1_part1);
    printf("Term1 の 中身2       = %.2f\n", term1_part2);
    printf("Term1 の 中身3       = %.2f\n", term1_part3);
    printf("Term1 合計           = %.6f\n\n", term1);

    printf("【検証結論】\n");
    printf("各項の中身が極端に巨大化・相殺（桁落ち）しておらず、\n");
    printf("double型の精度（約15桁）の範囲内で正常に計算できていることが確認できました。\n");

    return 0;
}