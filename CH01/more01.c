/* more01.c  - version 0.1 of more
 *    read and print 24 lines then pause for a few special commands
 */

#include <stdio.h>
#include <stdlib.h>
#define PAGELEN 24 //每次打印24行
#define LINELEN 512 //设置缓冲区大小

//domore将数据显示在显示器上
void do_more(FILE *);
//打印信息，等着响应用户命令' ', '\n' ,'q'，返回一个值(新增需要打印的行数) 或者 退出整个程序。
int  see_more();

int main( int ac , char *av[] )
{
    FILE *fp;

    if ( ac == 1 )      //如果参数个数是1(说明只有可执行文件arg[0]，未接收参数)，就从标准输入文件里读数据
        do_more( stdin );//调用domore
    else
        while ( --ac )//如果参数个数不是1，那就说明有命令行参数(这个while是不断打印各个文件的内容， arg[1],arg[2]...)
            if ( (fp = fopen( *++av , "r" )) != NULL ) //打开第一个文件名(C风格字符串)的文件 打开成功时
            {
                do_more( fp ) ; 
                fclose( fp );
            }
            else //打开失败，就退出。
                exit(1);
    return 0;
}

/*
 *  read PAGELEN lines, then call see_more() for further instructions
 */
void do_more( FILE *fp )  
{
    char line[LINELEN];
    //下一行将要打印的行索引 index。作为一个起点索引。( 需打印内容就是从 index~23行(含23) )
    int index_of_lines = 0;
    int reply;

    while ( fgets( line, LINELEN, fp ) ){        /* more input    */
        if ( index_of_lines == PAGELEN ) 
        {/* full screen?   屏幕有一屏了的情况下，就调用see_more函数,打印提示标记more。且接收用户的输入*/
            reply = see_more();        /* y: ask user 获得一个返回值，若是0，就退出程序。若是PAGELEN，就翻页（尽量打印PAGELEN行）。若是1,就打印1行。*/
            if ( reply == 0 )        /*    n: done   */
                break;
            index_of_lines -= reply;        /* reset count  若reply为1,则index = index-1,从23开始打印1行。若reply为PAGELEN, index = 0, 那就从0开始打印24行(一页) */
        }
        if ( fputs( line, stdout )  == EOF )    /* show line    */
            exit(1);            /* or die    */
        index_of_lines++;                /* count it 每成功打印一行，就计数+1, 而每次从用户那儿获得新的reply,计数index就-reply。*/
    }
}

/*
 *    print message, wait for response, return # of lines to advance
 *    q means no, space means yes, CR means one line
 */
int see_more()
{
    int c;

    printf("\033[7m more? \033[m");        /* reverse on a vt100    */
    while( (c=getchar()) != EOF )            /* get response    */
    {//若是这3个字符以外的任何字符，则自动跳过，进入下一次while
        if ( c == 'q' )            /* q -> N        */
            return 0;
        if ( c == ' ' )            /* ' ' => next page    */
            return PAGELEN;        /* how many to show    */
        if ( c == '\n' )        /* Enter key \n => 1 line    */
            return 1;        
    }
    return 0;
}
