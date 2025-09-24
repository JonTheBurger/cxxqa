#include <cxxqa/git.hpp>

int main()
{
  cxxqa::Git git;
  git.ls_files(cxxqa::Git::LS_CACHED);
  return 0;
}
