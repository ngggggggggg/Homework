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
## Polynomial 類別程式解題策略

1. **資料結構與初始化**  
   - 使用動態陣列 `Term* termArray` 儲存多項式的非零項，每個項包含係數 (`coef`) 與指數 (`exp`)。  
   - `capacity` 記錄陣列容量，`terms` 記錄目前非零項數。  
   - 建構子初始化容量為 10，解構子釋放動態記憶體，避免記憶體洩漏。  
   - 複製建構子用於回傳新多項式時的深拷貝。

2. **新增項目與排序 (`NewTerm`)**  
   - 若新增項係數為 0，直接略過。  
   - 若指數已存在，則合併係數；合併後若係數為 0，自動刪除該項。  
   - 若容量不足，自動將陣列擴充為原容量的兩倍。  
   - 新增項目後，依指數由大到小排序，方便輸出與運算。

3. **多項式加法 (`Add`)**  
   - 使用雙指標遍歷兩個多項式的項，對相同指數項相加。  
   - 不同次方的項直接加入結果多項式。  
   - 回傳新的多項式，保持原多項式不變。

4. **多項式乘法 (`Mult`)**  
   - 使用雙層迴圈將兩多項式的每個項相乘，生成新項。  
   - 新項加入結果多項式時，自動合併相同指數，確保結果多項式正確。  
   - 回傳乘積多項式。

5. **多項式代入計算 (`Eval`)**  
   - 遍歷所有項，將指定值代入 x 計算每項結果，並累加得到多項式的值。  

6. **輸入 (`>>`) 與輸出 (`<<`)**  
   - 輸入：先輸入多項式項數，再依序輸入每項的係數與指數，利用 `NewTerm` 自動管理。  
   - 輸出：依指數由大到小顯示，格式化正負號與次方，例如 `3x^2 - 2x + 1`。

7. **程式主流程 (`main`)**  
   - 輸入兩個多項式。  
   - 顯示原始多項式。  
   - 計算並顯示加法與乘法結果。  
   - 輸入 x 的值並計算兩多項式的代入結果。

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
## Polynomial 類別效能分析（整體觀點）

- **資料結構影響**  
  多項式使用動態陣列儲存非零項，每個項包含係數與指數。動態陣列在容量不足時會進行倍增，這使得新增項目的平均時間複雜度接近 O(1)，但在最壞情況下需要重新分配並複製整個陣列，時間為 O(n)。

- **新增與排序成本**  
  每次新增項目後都會按指數從大到小排序。隨著項數增加，排序操作可能成為主要瓶頸，最壞情況時間為 O(n^2)，尤其在多項式項數多、頻繁新增的情況下。

- **運算效能特性**  
  - 多項式加法使用雙指標遍歷兩個多項式，平均時間接近 O(m+n)，其中 m 與 n 為兩個多項式的項數，但每次呼叫新增項函數可能觸發排序或合併，最壞時間為 O(k^2)，k 為結果多項式項數。  
  - 多項式乘法需要對每對項進行運算，時間隨兩多項式項數的乘積增加，再加上合併同次項與排序，最壞情況時間接近 O(m*n*k)。

- **查詢與代入運算**  
  多項式代入計算僅遍歷項目，累加每項的值，時間複雜度線性 O(n)，空間額外需求很小，只需要累加變數。

- **輸入輸出效能**  
  輸入時，每項需呼叫新增項函數處理排序和合併，可能導致 O(n^2) 的最壞時間，但對於一般小規模多項式影響不大。  
  輸出則線性遍歷所有項，時間 O(n)。

- **總體觀察**  
  程式效能主要受動態陣列排序和新增項目合併的影響。若多項式項數大幅增加，排序成本會顯著影響加法與乘法效能。可以透過延遲排序或使用平衡資料結構（如 map）改善。

