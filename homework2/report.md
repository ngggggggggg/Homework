# 41343150
# Homework-2-Polynomial
# 解題說明
本題要求實作一個多項式 ADT（抽象資料型態）`Polynomial`，多項式由多個非零項 `Term` 組成，每個項包含浮點數係數 (`coef`) 與非負整數指數 (`exp`)，且需要支援以下功能：

1. **多項式建立與儲存**  
   使用動態陣列保存所有非零項，當新增項超過容量時自動擴充，確保可以存放任意數量的項。

2. **多項式加法 (Add)**  
   將兩個多項式的相同次方項合併，生成一個新的多項式，保持原多項式不變。

3. **多項式乘法 (Mult)**  
   對兩個多項式的每個項進行乘法，將生成的新項自動合併相同次方，產生乘積多項式。

4. **多項式代入計算 (Eval)**  
   將指定數值代入 x，計算多項式在該數值下的結果並回傳。

5. **多項式輸出顯示 (Display/Show)**  
   將多項式按指數從大到小排列，正負號與次方格式化顯示，例如：`3x^2 - 2x + 1`。
# 解題策略
**資料結構與初始化:**
使用動態陣列 termArray 儲存非零項，以 
capacity 和 terms 負責管理大小，建構子設容量 10，解構子釋放記憶體，複製建構子來進行拷貝。

**新增項與排序 (NewTerm):**
若係數為 0 就跳過，指數相同則合併，容量不足自動擴充，最後依指數降冪排序。

**多項式加法 (Add):**
以雙指標逐項比較，同次項相加，不同次項直接加入新多項式。

**多項式乘法 (Mult):**
雙層迴圈逐項相乘並自動合併同指數項，形成正確乘積結果。

**多項式代入計算 (Eval):**
逐項計算 coef * pow(x, exp) 並加總得到多項式值。

**輸入與輸出:**
輸入時給項數與各項資料並用 NewTerm 管理，輸出時降冪顯示並處理正負號格式。

**主程式流程 (main):**
輸入兩多項式，顯示後計算加法與乘法，再輸入 x 求各自代入值。

# 程式實作
#include <iostream>
#include <cmath>
using namespace std;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
private:
    float coef;
    int exp;
};

class Polynomial {
    friend istream& operator>>(istream&, Polynomial&);
    friend ostream& operator<<(ostream&, const Polynomial&);

private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }

    Polynomial(const Polynomial& other) {
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = other.termArray[i];
    }

    ~Polynomial() { delete[] termArray; }

    void NewTerm(float c, int e) {
        if (c == 0) return;
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                if (termArray[i].coef == 0) {
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;
        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    Polynomial Add(Polynomial poly) {
        Polynomial result;
        int i = 0, j = 0;
        while (i < terms && j < poly.terms) {
            if (termArray[i].exp == poly.termArray[j].exp) {
                float sum = termArray[i].coef + poly.termArray[j].coef;
                if (sum != 0) result.NewTerm(sum, termArray[i].exp);
                i++; j++;
            }
            else if (termArray[i].exp > poly.termArray[j].exp) {
                result.NewTerm(termArray[i].coef, termArray[i].exp);
                i++;
            }
            else {
                result.NewTerm(poly.termArray[j].coef, poly.termArray[j].exp);
                j++;
            }
        }
        for (; i < terms; i++)
            result.NewTerm(termArray[i].coef, termArray[i].exp);
        for (; j < poly.terms; j++)
            result.NewTerm(poly.termArray[j].coef, poly.termArray[j].exp);
        return result;
    }

    Polynomial Mult(Polynomial poly) {
        Polynomial result;
        for (int i = 0; i < terms; i++) {
            for (int j = 0; j < poly.terms; j++) {
                float c = termArray[i].coef * poly.termArray[j].coef;
                int e = termArray[i].exp + poly.termArray[j].exp;
                result.NewTerm(c, e);
            }
        }
        return result;
    }

    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};

istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    cout << "請輸入項數: ";
    in >> n;
    poly.terms = 0;
    for (int i = 0; i < n; i++) {
        float c;
        int e;
        cout << "請輸入第 " << i + 1 << " 項的係數與指數: ";
        in >> c >> e;
        poly.NewTerm(c, e);
    }
    return in;
}

ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }
    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;
        if (i > 0) {
            if (c >= 0) out << " + ";
            else { out << " - "; c = -c; }
        }
        else if (c < 0) {
            out << "-";
            c = -c;
        }
        if (e == 0)
            out << c;
        else if (e == 1)
            out << c << "x";
        else
            out << c << "x^" << e;
    }
    return out;
}

int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式:\n";
    cin >> p1;
    cout << "輸入第二個多項式:\n";
    cin >> p2;

    cout << "p1(x) = " << p1 << endl;
    cout << "p2(x) = " << p2 << endl;

    Polynomial sum = p1.Add(p2);
    Polynomial product = p1.Mult(p2);

    cout << "p1(x) + p2(x) = " << sum << endl;
    cout << "p1(x) * p2(x) = " << product << endl;

    float x;
    cout << "請輸入 x 的值: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;
    cout << "p2(" << x << ") = " << p2.Eval(x) << endl;
    return 0;
}
# 效能分析
**Add 函式**:使用雙指標依序掃描兩個多項式進行合併，項數越多效率會逐步下降。

**Mult 函式**:使用兩層迴圈逐個項相乘，並在內層合併結果，項數越多效率會逐步下降。

**Eval 函式**:對每個項計算冪次後累加，項數越多效率會逐步下降。

**NewTerm 函式**:每次插入新一項都會重新排序，並合併同次項，項數越多效率會逐步下降。


