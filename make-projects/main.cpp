#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;

const int methods_per_class = 2;
const int fields_per_class = 2;
const int class_per_file = 2;
const int count_A_files_perB = 2;
const int count_B_files = 2;
const int count_proj_files = 10;

void gen_one_big_file(ostream &f) {
    f << "export module Funcs;" << endl;
    for (int i=0; i<100000; i++) {
        f << "export int f"<<i<<"() {" << endl;
        f << " return "<<i <<";"<< endl;
        f << "}" << endl;
    }
}

void gen_method_body(ostream &f, int k) {
    f << "{" << endl;
    f << " return n+"<< k <<";"<< endl;
    f << "}" << endl;
}

void gen_method_declaration(ostream &f,bool export_, string methodName) {
    if (export_)
        f << "export ";
    f << "int " <<  methodName << "(int n);" << endl;
}

void gen_method(ostream &f,bool export_, string className, string methodName, int k) {
    if (export_)
        f << "export ";
    f << "int " << className << "::" << methodName << "(int n)" << endl;
    gen_method_body(f,k);
}

void gen_method_inline(ostream &f, bool export_,string methodName, int k) {
    if (export_)
        f << "export ";
    f << "int " <<  methodName << "(int n)" << endl;
    gen_method_body(f,k);
}

void gen_fields(ostream &f, bool export_) {
    for (int i=0; i<fields_per_class;i++)
    {
        if (export_)
            f << "export ";
        f << "int field" << i<< ";" << endl;
    }
}

void gen_class_header(ostream &f, bool export_, string className) {
    if (export_)
        f << "export ";
    f << "class " << className << " {" << endl;
    gen_fields(f, export_);
    for (int i=0; i<methods_per_class; i++)
        gen_method_declaration(f, export_, "method_"+to_string(i));
    f << "};" << endl;
}

void gen_class_inline(ostream &f, bool export_, string className) {
    if (export_)
        f << "export ";
    f << "class " << className << " {" << endl;
    gen_fields(f, export_);
    for (int i=0; i<methods_per_class; i++)
        gen_method_inline(f, export_, "method_"+to_string(i) ,i);
    f << "};" << endl;
}


void gen_file_inlineA(string dirName, int k) {
    ofstream f(dirName+'/'+"a"+to_string(k)+".h");
    for (int i=0; i<class_per_file; i++)
        gen_class_inline(f,  false, "class_a"+to_string(k)+"_"+to_string(i));
}

void gen_file_headerB(string dirName, int k) {
    ofstream f(dirName+'/'+"b"+to_string(k)+".h");
    for (int i=0; i<count_A_files_perB; i++)
        f << "#include \"a" << k*count_A_files_perB+ i <<".h\"" <<endl;
    for (int i=0; i<class_per_file; i++)
        gen_class_header(f,  false, "class_b"+to_string(k)+"_"+to_string(i));
}

void gen_file_sourceB(bool isModule, string dirName, int k) {
    ofstream f(dirName+'/'+"b"+to_string(k)+".cpp");
    if (isModule)
        f << "export module b"<<k<< endl;
    f << "#include \"b"<<k<<".h\"" <<endl;
    for (int i=0; i<class_per_file; i++)
        for (int j=0; j<methods_per_class; j++)
            gen_method(f,  false, "class_b"+to_string(k)+"_"+to_string(i), "method_"+to_string(j), j);
}

void gen_file_source_proj(bool isModule, string dirName, int k) {
    ofstream f(dirName+'/'+"p"+to_string(k)+".cpp");
    if (isModule) {
        f << "export module p" << k << endl;
        for (int i = 0; i < count_B_files; i++)
            f << "import b" << i << endl;
    } else {
        for (int i = 0; i < count_B_files; i++)
            f << "#include \"b" << i << ".h\"" << endl;
    }
    for (int i=0; i<class_per_file; i++)
        for (int j=0; j<methods_per_class; j++)
            gen_method(f,  false, "class_"+to_string(i), "method_"+to_string(j), j);
}

void genLibCmake(string dirName, bool isModule) {
    ofstream f(dirName+'/'+"CMakeLists.txt");
    string projName = "stdlib";
    string minVer;
    if (isModule) {
        minVer = "3.26";
        projName += 'M';
    }
    else minVer = "3.20";

    f << "cmake_minimum_required(VERSION "<<minVer<<")" << endl;
    f << "project("<<projName<<")"<<endl;
    if (isModule)
        f << "set(CMAKE_CXX_STANDARD 11)"<<endl;
    else
        f << "set(CMAKE_CXX_STANDARD 20)"<<endl;
    f << "add_library(${PROJECT_NAME} STATIC"<<endl;
    for (int i=0; i<count_B_files; i++)
        f << "  b"<<i<<".cpp"<<endl;
    f << ")"<<endl;
}

void gen_stdlib(bool isModule) {
    string dirName = "stdlib";
    if (isModule)
        dirName+='M';
    boost::filesystem::create_directory(dirName);
    for (int i=0; i<count_B_files; i++) {
        gen_file_sourceB(isModule, dirName, i);
        gen_file_headerB(dirName, i);
        for (int j=0; j<count_A_files_perB; j++){
            gen_file_inlineA(dirName, i*count_A_files_perB+j);
        }
    }
    genLibCmake(dirName, isModule);
}

void gen_main(bool isModule, string dirName) {
    ofstream f(dirName+'/'+"main.cpp");
    if (isModule)
        for (int i=0; i<count_proj_files; i++)
            f << "import p"<<i<<";" << endl;
    else
        for (int i=0; i<count_B_files; i++)
            f << "#include \"b"<<i<<"\""<< endl;
    f << "int main(){}"<< endl;
}

void gen_proj(bool isModule) {
    string dirName = "proj";
    if (isModule)
        dirName+='M';
    boost::filesystem::create_directory(dirName);
    for (int i=0; i < count_proj_files; i++)
        gen_file_source_proj(isModule, dirName, i);
    gen_main(isModule, dirName);
}

int main() {
    gen_stdlib(false);
    gen_stdlib(true);
    gen_proj(false);
    gen_proj(true);
    return 0;
}
