//created by DeepSeek online service
// 核素数据结构
struct Nuclide {
    int A;
    int Z;
    string element;
    double mass_excess;
};

// 解析单行数据,one-line processing
bool parseLine(const string& line, Nuclide& result) {
    // 跳过控制字符和短行
    if (line.length() < 55) return false;

    // 固定列格式解析
    try {
        // 提取关键字段（基于文件格式描述）
        int NZ = stoi(line.substr(1, 3));   // 列2-4: NZ
        int N = stoi(line.substr(4, 5));    // 列5-9: N
        int Z = stoi(line.substr(9, 5));    // 列10-14: Z
        int A = stoi(line.substr(14, 5));   // 列15-19: A
        string el = line.substr(20, 3);     // 列21-23: 元素符号
        string mass_str = line.substr(28, 14); // 列29-42: 质量过剩（keV）

        // 清理元素符号中的空格
        el.erase(remove(el.begin(), el.end(), ' '), el.end());
        if (el.empty()) return false;

        // 处理质量过剩中的特殊字符（如#和*）
        size_t invalid_pos = mass_str.find_first_of("#*");
        if (invalid_pos != string::npos) {
            mass_str = mass_str.substr(0, invalid_pos);
        }
        if (mass_str.empty()) return false;

        // 转换为数值
        double mass_excess = stod(mass_str);

        // 填充结果
        result.A = A;
        result.Z = Z;
        result.element = el;
        result.mass_excess = mass_excess;
        return true;

    } catch (...) {
        return false; // 解析失败
    }
}

// 主函数
void ameMass2020(int a, int z){
	const char massFile[50]="mass_1.mass_1.txt";//https://www-nds.iaea.org/amdc/ame2020/mass_1.mas20.txt
    ifstream file(massFile);
    if (!file) {
        cerr << "Error: Cannot open "<<massFile <<"\n";
        return;
    }

    string line;
    bool in_data_section = false;

    // 跳过文件头，定位到数据区域
    while (getline(file, line)) {
        if (line.find("MASS EXCESS") != string::npos) {
            in_data_section = true;
            getline(file, line); // 跳过列标题行
            break;
        }
    }

    if (!in_data_section) {
        cerr << "Error: Data section not found" << endl;
        return;
    }

    // 逐行搜索目标核素
    while (getline(file, line)) {
        Nuclide nuclide;
        if (parseLine(line, nuclide)) {
            if (nuclide.A == a && nuclide.Z == z) {
                cout << "nuclide: " << nuclide.element 
                     << " mass excess: " << nuclide.mass_excess << " keV" << endl;
                return;
            }
        }
    }
    cerr << "failed to find A=" << a << " Z=" << z<< endl;
}
