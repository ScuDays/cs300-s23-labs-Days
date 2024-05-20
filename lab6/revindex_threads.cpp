#include <thread>

#include "wordindex.h"

using namespace std;

/*TODO
 * Fill in this function so that it creates a series of threads to execute
 * find_word over different files in parallel.
 *
 * Arguments:
 *     - files: a reference to a vector of wordindex class objects meant to hold
 *       the search results of all files.
 *     - filenames: a vector containing strings of all of filenames in the
 *       directory,
 *     - start_pos: an integer representing the index of the file in filenames
 *       that this batch of threads should start with.
 *     - num_threads: an integer representing the number of threads that should
 *       be created in this round.
 *     - word: a string containing the desired search term.
 * Return value: an integer representing the total occurrences of the search
 * term in this batch of files.

 * 填写这个函数，使其创建一系列线程并行执行`find_word`
函数来搜索不同文件中的内容。 参数：
- files: 一个指向 wordindex 类对象的向量引用，用于保存所有文件的搜索结果。
- filenames: 包含目录中所有文件名字符串的向量，
- start_pos: 一个整数，表示该批线程应从filenames 中哪个索引开始处理。
- num_threads: 一个整数，表示此轮应创建多少个线程。
- word: 包含所需搜索词条的字符串。

返回值：代表这批文件中搜索词条总出现次数的整数。
 */
int run_workers(vector<wordindex>& files, vector<string> filenames,
                int start_pos, int num_threads, string word) {
  // for each thread in num_threads:
  // 1) create a new index object for the file being processes by the thread and
  //    add it to the files vector
  // 2) create the thread to run find_word with the proper arguments
  // 对于每个线程num_threads：
  // 1）为正在处理的文件创建一个新的索引对象，并将其添加到文件向量中
  // 2）使用适当的参数创建线程来运行find_word

  // 查询到的总数
  int num_occurrences = 0;
  std::thread threadArr[num_threads];
  wordindex fileArr[num_threads];
  for (int i = 0; i < num_threads; i++) {
    fileArr[i].filename = filenames[i + start_pos];
    threadArr[i] = thread(find_word, &fileArr[i], word);
  }
  for (int i = 0; i < num_threads; i++) {
    threadArr[i].join();
    num_occurrences += fileArr[i].indexes.size();
    files.push_back(fileArr[i]);
  }
  // join with each thread and add the count field of each index to the total
  // 与每个线程一起连接，并将每个索引的计数字段添加到总和中
  // return the total sum for this batch of files
  // 返回此批文件的总和
  return num_occurrences;
}

/* The main REPL for the program
 * continually reads input for stdin, searches for that input in a directory of
 * files, and prints the results
 *
 * Arguments: char* containing the name of the  * directory to search
 *            through, dirname
 * Return value: None
 */
void repl(char* dirname) {
  int num_files = 0;
  int workers = 8;
  vector<wordindex> files;
  vector<string> filenames;

  // read all files from directory
  get_files(filenames, dirname);
  // 需要查询的文件总数
  num_files = filenames.size();

  files.reserve(num_files);

  printf("Enter search term: ");
  char buf[255];
  fgets(buf, 255, stdin);
  while (!feof(stdin)) {
    // 将char数组转为string字符串
    int len = char_traits<char>::length(buf) - 1;
    string term(buf, len);
    if (term.length() > 0) {
      int num_occurrences = 0;
      // 将所有字母转为小写字母
      transform(term.begin(), term.end(), term.begin(), ::tolower);
      // 已完成查询任务数
      int completed = 0;
      while (completed < num_files) {
        // determine number of threads for this round
        int num_threads = 0;
        if ((num_files - completed) > workers) {
          num_threads = workers;
        } else {
          num_threads = num_files - completed;
        }

        // increment the total number of occurrences of the search term by the
        // sum of the occurrences from this batch of files files is passed by
        // reference so that additions to the vector in run_workers is visable
        // back in this function
        num_occurrences +=
            run_workers(files, filenames, completed, num_threads, term);

        // update the number of completed files
        completed += num_threads;

        // move on to next batch of "workers" amount of files
      }

      // print the results
      printOccurrences(term, num_occurrences, files);
    }

    printf("\nEnter search term: ");
    files.clear();

    fgets(buf, 255, stdin);
  }
  return;
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
