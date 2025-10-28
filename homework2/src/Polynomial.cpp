#include <iostream>
#include <cmath>
using namespace std;

// ------------------ Term 類別 ------------------
class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream&, const Polynomial&);
private:
    float coef;  // 係數
    int exp;     // 指數
};

// ------------------ Polynomial 類別 ------------------
class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    Term* termArray;   // 儲存多項式的項
    int capacity;      // 陣列容量
    int terms;         // 實際項數

public:
    // 建構子
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    // ✅ 解構子：釋放動態記憶體
    ~Polynomial() {
        delete[] termArray;
    }

    // 新增一個項目 (自動合併同次項 + 排序)
    void NewTerm(float c, int e) {
        if (c == 0) return;

        // 檢查是否已有相同次方 → 合併
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                return;
            }
        }

        // 若容量滿了 → 擴充陣列
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }

        // 新增項目
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;

        // 按指數遞減排序
        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    // 計算多項式值 p(x)
    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};

// ------------------ operator>> (輸入多項式) ------------------
istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    cout << "Enter number of terms: ";
    in >> n;

    poly.terms = 0; // 清空原本內容

    for (int i = 0; i < n; i++) {
        float c;
        int e;
        cout << "Enter coefficient and exponent for term " << i + 1 << ": ";
        in >> c >> e;
        poly.NewTerm(c, e);
    }

    return in;
}

// ------------------ operator<< (輸出多項式) ------------------
ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }

    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;

        // 處理正負號
        if (i > 0) {
            if (c >= 0) out << " + ";
            else { out << " - "; c = -c; }
        }
        else if (c < 0) {
            out << "-";
            c = -c;
        }

        // 處理格式
        if (e == 0)
            out << c;
        else if (e == 1)
            out << c << "x";
        else
            out << c << "x^" << e;
    }

    return out;
}

// ------------------ 主程式測試 ------------------
int main() {
    Polynomial p;

    // 輸入多項式
    cin >> p;

    // 顯示多項式
    cout << "p(x) = " << p << endl;

    // 輸入 x 並計算 p(x)
    float x;
    cout << "Enter x value: ";
    cin >> x;
    cout << "p(" << x << ") = " << p.Eval(x) << endl;

    return 0;
}

