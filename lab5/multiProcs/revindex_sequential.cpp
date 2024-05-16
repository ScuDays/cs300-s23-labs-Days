#include <fcntl.h>
#include <sys/mman.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include "wordindex.h"

using namespace std;

int process_input(string term, vector<string>& filenames) {
  // 获得文件数量
  int workers = filenames.size();
  int num_occurrences = 0;

  // wordindex —— 包含搜索结果的结构体
  vector<wordindex> fls;
  // 遍历查找所有文件
  for (int w = 0; w < workers; w++) {
    wordindex file;
    file.filename = filenames[w];
  // 查找函数，第一个参数是在哪个文件里面查找，第二个参数是查询什么词语
    find_word(&file, term);
    num_occurrences += file.indexes.size();
    fls.push_back(file);
  }
  // 打印出结果
  printOccurrences(term, num_occurrences, fls);
  return 0;
}

void repl(char* dirname) {
  // read dir
  // 存储所有需要查找的文件
  vector<string> filenames;
  // 查找指定文件夹内所有文件名字
  get_files(filenames, dirname);

  printf("Enter search term: ");
  // 循环读取输入的查询词，并查询
  for (string term = ""; getline(cin, term);) {
    int err = process_input(term, filenames);
    if (err < 0) {
      printf("%s: %d\n", "ERROR", err);
    }
    printf("\nEnter search term: ");
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Expected arguments: files to search through\n");
    exit(1);
  }

  char* dirname = argv[1];

  repl(dirname);

  exit(0);
}
