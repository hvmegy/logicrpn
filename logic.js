function run() {
    // Reset
    RPN = "";
    sto = new Set();
    str = "";
    anhxa = {};
    Dung = true;
    Sai = true;
    document.getElementById("output").innerHTML = "";
    document.getElementById("resultText").innerText = "";

    N = document.getElementById("exprInput").value;
    if (!N) {
        document.getElementById("resultText").innerText = "Vui lòng nhập biểu thức.";
        return;
    }

    try {
        chuyenDoi();
        let html = lapBang(); // tạo bảng HTML
        document.getElementById("output").innerHTML = html;

        if (Dung) document.getElementById("resultText").innerText = "➤ Hằng Đúng";
        else if (Sai) document.getElementById("resultText").innerText = "➤ Hằng Sai";
        else document.getElementById("resultText").innerText = "➤ Không Hằng Đúng Cũng Không Hằng Sai";
    } catch (e) {
        document.getElementById("resultText").innerText = "❌ Lỗi cú pháp: " + e.message;
    }
}

function lapBang() {
    let rows = [];
    let header = "<tr>";
    for (let c of str) header += `<th>${c}</th>`;
    header += "<th>F</th></tr>";
    rows.push(header);

    function back(ix, permu) {
        if (ix >= n - 1) {
            for (let i = 0; i < n; i++) {
                anhxa[str[i]] = permu[i] === '1';
            }

            let res = f();
            if (res) Sai = false;
            else Dung = false;

            let row = "<tr>";
            for (let i = 0; i < n; i++) {
                row += `<td>${anhxa[str[i]] ? 1 : 0}</td>`;
            }
            row += `<td>${res ? 1 : 0}</td></tr>`;
            rows.push(row);
            return;
        }

        back(ix + 1, permu + "0");
        back(ix + 1, permu + "1");
    }

    back(-1, "");
    return `<table>${rows.join("")}</table>`;
}

function chuyenDoi() {
    let st = [];
    let qu = [];
    let cnt = 0;
    let prio = {
        '!': 3,
        '&': 2,
        '|': 2,
        '-': 2, // dùng để biểu diễn -> : p - q = !p | q
        '(': 0
    };
    let bu = Array(2e5).fill(0);

    for (let i = 0; i < N.length; i++) {
        let ch = N[i];
        if ((ch >= 'a' && ch <= 'z') || ch === '1' || ch === '0') {
            qu.push(ch);
            while (bu[cnt] > 0) {
                qu.push('!');
                bu[cnt]--;
            }
            if (ch >= 'a' && ch <= 'z') sto.add(ch);
        } else {
            if (ch !== ')') {
                if (ch === '!') {
                    bu[cnt]++;
                } else if (st.length === 0 || ch === '(') {
                    st.push(ch);
                } else {
                    while (st.length > 0 && prio[st[st.length - 1]] >= prio[ch]) {
                        qu.push(st.pop());
                    }
                    st.push(ch);
                }
            } else {
                while (st[st.length - 1] !== '(') {
                    qu.push(st.pop());
                }
                cnt--;
                while (bu[cnt] > 0) {
                    qu.push('!');
                    bu[cnt]--;
                }
                st.pop(); // bỏ '('
            }
            if (ch === '(') cnt++;
        }
    }

    while (st.length > 0) {
        qu.push(st.pop());
    }

    RPN = qu.join('');
    str = Array.from(sto).join('');
    n = str.length;
}

function f() {
    let st = [];
    for (let c of RPN) {
        if (c >= 'a' && c <= 'z') {
            st.push(anhxa[c]);
        } else if (c === '1' || c === '0') {
            st.push(c === '1');
        } else {
            if (c === '&') {
                let a = st.pop();
                let b = st.pop();
                st.push(b && a);
            }
            if (c === '|') {
                let a = st.pop();
                let b = st.pop();
                st.push(b || a);
            }
            if (c === '-') { 
                let a = st.pop();
                let b = st.pop();
                st.push(!b || a);
            }
            if (c === '!') {
                let a = st.pop();
                st.push(!a);
            }
        }
    }
    return st.pop();
}
