/**
 * @page DocTag_FAQ FAQ
 *
 * -  <b>Q</b>: why it takes so long to build\n
 *   <b>A</b>: may be:
 *   -  we need to git clone some repos to build the project,
 *     which may cost much time accorrding to your network
 *   -  the C++ itself takes long time to compile
 *
 *   once you successfully build the library,
 *   it should be faster for the second time
 * -  <b>Q</b>: compile error\n
 *   <b>A</b>: try running cleanup.sh/cleanup.bat and build again\n
 *   if error still occurred:
 *   -# try running "~/zf3rd_setup.sh" to check whether it setup properly
 *   -# try running proper release script under "~/tools/release",
 *     and check the error message
 */
