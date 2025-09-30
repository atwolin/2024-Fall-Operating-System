// #include "syscall.h"

// int main(void) {
//     int i;
//     int success = Create("file1.test");
//     int test0 = Create("test0.test");
//     int test1 = Create("test1.test");
//     int test2 = Create("test2.test");
//     int test3 = Create("test3.test");
//     int test4 = Create("test4.test");
//     int test5 = Create("test5.test");
//     int test6 = Create("test6.test");
//     int test7 = Create("test7.test");
//     int test8 = Create("test8.test");
//     int test9 = Create("test9.test");
//     int test10 = Create("test10.test");
//     int test11 = Create("test11.test");
//     int test12 = Create("test12.test");
//     int test13 = Create("test13.test");
//     int test14 = Create("test14.test");
//     int test15 = Create("test15.test");
//     int test16 = Create("test16.test");
//     int test17 = Create("test17.test");
//     int test18 = Create("test18.test");
//     int test19 = Create("test19.test");
//     OpenFileId fid, duplicate, non_existent, exceeded, open_file0, open_file1, open_file2, open_file3, open_file4, open_file5, open_file6, open_file7, open_file8, open_file9, open_file10, open_file11, open_file12, open_file13, open_file14, open_file15, open_file16, open_file17, open_file18;
//     char test[] = "abcdefghijklmnopqrstuvwxyz";

//     if (success != 1)
//         MSG("Failed on creating file");
//     if (test0 != 1)
//         MSG("Failed on creating test0");
//     if (test1 != 1)
//         MSG("Failed on creating test1");
//     if (test2 != 1)
//         MSG("Failed on creating test2");
//     if (test3 != 1)
//         MSG("Failed on creating test3");
//     if (test4 != 1)
//         MSG("Failed on creating test4");
//     if (test5 != 1)
//         MSG("Failed on creating test5");
//     if (test6 != 1)
//         MSG("Failed on creating test6");
//     if (test7 != 1)
//         MSG("Failed on creating test7");
//     if (test8 != 1)
//         MSG("Failed on creating test8");
//     if (test9 != 1)
//         MSG("Failed on creating test9");
//     if (test10 != 1)
//         MSG("Failed on creating test10");
//     if (test11 != 1)
//         MSG("Failed on creating test11");
//     if (test12 != 1)
//         MSG("Failed on creating test12");
//     if (test13 != 1)
//         MSG("Failed on creating test13");
//     if (test14 != 1)
//         MSG("Failed on creating test14");
//     if (test15 != 1)
//         MSG("Failed on creating test15");
//     if (test16 != 1)
//         MSG("Failed on creating test16");
//     if (test17 != 1)
//         MSG("Failed on creating test17");
//     if (test18 != 1)
//         MSG("Failed on creating test18");
//     if (test19 != 1)
//         MSG("Failed on creating test19");

//     fid = Open("file1.test");
//     if (fid < 0)
//         MSG("Failed on opening file");
//     duplicate = Open("file1.test");
//     if (duplicate != -1)
//         MSG("Failed to handle duplicate file opening");
//     non_existent = Open("file2.test");
//     if (non_existent != -1)
//         MSG("Failed to handle non-existent file opening");
//     open_file0 = Open("test0.test");
//     if(open_file0 == -1) 
//         MSG("Failed on opening test0");
//     open_file1 = Open("test1.test");
//     if(open_file1 == -1) 
//         MSG("Failed on opening test1");
//     open_file2 = Open("test2.test");
//     if(open_file2 == -1) 
//         MSG("Failed on opening test2");
//     open_file3 = Open("test3.test");
//     if(open_file3 == -1) 
//         MSG("Failed on opening test3");
//     open_file4 = Open("test4.test");
//     if(open_file4 == -1) 
//         MSG("Failed on opening test4");
//     open_file5 = Open("test5.test");
//     if(open_file5 == -1) 
//         MSG("Failed on opening test5");
//     open_file6 = Open("test6.test");
//     if(open_file6 == -1) 
//         MSG("Failed on opening test6");
//     open_file7 = Open("test7.test");
//     if(open_file7 == -1) 
//         MSG("Failed on opening test7");
//     open_file8 = Open("test8.test");
//     if(open_file8 == -1) 
//         MSG("Failed on opening test8");
//     open_file9 = Open("test9.test");
//     if(open_file9 == -1) 
//         MSG("Failed on opening test9");
//     open_file10 = Open("test10.test");
//     if(open_file10 == -1) 
//         MSG("Failed on opening test10");
//     open_file11 = Open("test11.test");
//     if(open_file11 == -1) 
//         MSG("Failed on opening test11");
//     open_file12 = Open("test12.test");
//     if(open_file12 == -1) 
//         MSG("Failed on opening test12");
//     open_file13 = Open("test13.test");
//     if(open_file13 == -1) 
//         MSG("Failed on opening test13");
//     open_file14 = Open("test14.test");
//     if(open_file14 == -1) 
//         MSG("Failed on opening test14");
//     // test14 = Close(open_file14);
//     // if(test14 == 1) {
//     //     PrintInt(1126);
//     // }
//     open_file15 = Open("test15.test");
//     if(open_file15 == -1) 
//         MSG("Failed on opening test15");
//     open_file16 = Open("test16.test");
//     if(open_file16 == -1) 
//         MSG("Failed on opening test16");
//     open_file17 = Open("test17.test");
//     if(open_file17 == -1) 
//         MSG("Failed on opening test17");
//     open_file18 = Open("test18.test");
//     if(open_file18 == -1) 
//         MSG("Failed on opening test18");
//     test18 = Close(open_file18);
//     if(test18 != 1) {
//         MSG("Failed on closing test18");
//     }
//     exceeded = Open("file19.test");
//     if(exceeded == -1)
//         // MSG("Failed to handle exceeded file opening.");
//         PrintInt(1127);
//     // exceeded = Open("file19.test");
//     // if(exceeded == -1)
//     //     // MSG("Failed to handle exceeded file opening.");
//     // exceeded = Open("file19.test");
//     // if(exceeded != -1)
//     //     MSG("Failed to handle exceeded file opening.");
//     // test18 = Close(open_file18);
//     // if(test18 == 1) {
//     //     PrintInt(1126);
//     // }
//     // exceeded = Open("file19.test");
//     // if(exceeded == -1)
//     //     MSG("Failed to handle exceeded file opening!!.");
//     // for (i = 0; i < 26; ++i) {
//     //     int count = Write(test + i, 1, fid);
//     //     if (count != 1)
//     //         MSG("Failed on writing file");
//     // }
//     // for (i = 0; i < 13; ++i) {
//     //     int count = Write(test + 2 * i, 2, fid);
//     //     if (count != 2) MSG("Failed on writing file");
//     // }
//     // int not_yet_opened = Write(test + 1, 1, open_file[1]);
//     // if (not_yet_opened != -1)
//     //     MSG("Failed to handle not-yet-opened file writing");
//     // int invalid_id = Write(test + 1, 1, -2);
//     // if (invalid_id != -1)
//     //     MSG("Failed to handle invalid(< 0) file writing");
//     // invalid_id = Write(test + 1, 1, 20);
//     // if (invalid_id != -1)
//     //     MSG("Failed to handle invalid(>= 20) file writing");

//     success = Close(fid);
//     if (success != 1)
//         MSG("Failed on closing file");
//     // not_yet_opened = Close(17);
//     // if (not_yet_opened != -1)
//     //     MSG("Failed to handle not-yet-opened file closing");
//     // invalid_id = Close(-2);
//     // if (invalid_id != -1)
//     //     MSG("Failed to handle invalid(< 0) file closing");
//     // invalid_id = Close(20);
//     // if (invalid_id != -1)
//     //     MSG("Failed to handle invalid(>= 20) file closing");

//     MSG("Success on creating file1.test");
//     Halt();
// }

#include "syscall.h"

int main(void) {
    char test[] = "abcdefghijklmnopqrstuvwxyz";
    int success = Create("file1.test");
    OpenFileId fid;
    int i;
    if (success != 1)
        MSG("Failed on creating file");
    fid = Open("file1.test");

    if (fid < 0)
        MSG("Failed on opening file");

    for (i = 0; i < 26; ++i) {
        int count = Write(test + i, 1, fid);
        if (count != 1)
            MSG("Failed on writing file");
    }

    success = Close(fid);
    if (success != 1)
        MSG("Failed on closing file");
    MSG("Success on creating file1.test");
    Halt();
}


// #include "syscall.h"

// int main(void) {
//     char test[] = "abcdefghijklmnopqrstuvwxyz";
//     int success = Create("file1.test");
//     OpenFileId fid, fid2;
//     int i;
//     if (success != 1) MSG("Failed on creating file");
//     fid = Open("file1.test");

//     if (fid < 0) MSG("Failed on opening file");

//     // Valid id
//     // int count = 0;
//     // count = Write(test, 2, fid);
//     // if (count == 2) MSG("Valid :)");

//     // Invalid id
//     // int invalid = Write(test, 2, 21);
//     // if (invalid == -1) MSG("Invalid ID from test1!");

//     // Non-exist id
//     // int non = Write(test, 2, 6);
//     // if (non == -1) MSG("Non-exist ID :(");

//     // for (i = 0; i < 13; ++i) {
//     //     int count = Write(test + 2 * i, 2, fid);
//     //     if (count != 2) MSG("Failed on writing file");
//     //     PrintInt(count);
//     // }

//     for (i = 0; i < 26; ++i) {
//         int count = Write(test + i, 1, fid);
//         if (count != 1) MSG("Failed on writing file:O");
//         PrintInt(count);
//     }

//     fid2 = Open("file1.test");
//     // PrintInt(fid2);
//     success = Close(fid);
//     if (success != 1) MSG("Failed on closing file");

//     if (fid2 == -1) PrintInt(fid2);

//     MSG("Success on creating file1.test");
//     MSG("Success on creating file1.tes:)))");
//     Halt();
// }
