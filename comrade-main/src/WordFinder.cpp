#include "WordFinder.h"

Scorpio::WordFinder::WordFinder()
{
    string_handle=NULL;
    ptr=0;
    is_valid=false;
    success=false;

    //cout<<"WordFinder object created...\n";
}

void Scorpio::WordFinder::reset()
{
    if (is_valid==true)
    {
        delete[] string_handle;
    }

    string_handle=NULL;
    ptr=0;
    is_valid=false;
    success=false;
}

void Scorpio::WordFinder::set_ptr(int p)
{
    ptr=p;
}

int Scorpio::WordFinder::return_ptr()
{
    return ptr;
}

void Scorpio::WordFinder::assign_string(char* str)
{
    if (is_valid==true)
    {
         delete[] string_handle;
    }

    else
    {
         is_valid=true;
    }

    string_handle=new char[strlen(str)+1];
    strcpy(string_handle,str);
    string_handle[strlen(str)]='\0';

    ptr=0;
    success=false;
}

char Scorpio::WordFinder::find_next_nonspace()
{
    int copy=ptr;

    while (string_handle[copy]==' ')
    {
        copy++;
    }

    return string_handle[copy];
}

void Scorpio::WordFinder::find_next_word(char* result)
{
    int end=0,start=0;

    if (string_handle[ptr]!='\0')
    {
        while (string_handle[ptr]==' ' || string_handle[ptr]==';')
        {
            //if (string_handle[ptr]==';')
            //{
            //    cout<<"Have encountered a semicolon....\n";
            //}

            ptr++;
        }

        start=ptr;

        while (string_handle[ptr]!=' ' && string_handle[ptr]!=';' && string_handle[ptr]!='\0')
        {
            ptr++;
            //cout<<"Going for a space...second while"<<ptr<<endl;
        }

        end=ptr-1;


        //cout<<start<<" "<<end;

        if (string_handle[start]!='\0')
        {
            success=true;

            for (int i=0; i<=end-start; i++)
            {
                result[i]=string_handle[start+i];
            }

            result[end-start+1]='\0';
            //cout<<"Is this okay? "<<result<<endl;
        }

        else
        {
            success=false;
        }
    }

    else
    {
        success=false;
    }
}

void Scorpio::WordFinder::find_next_word(int& num)
{
    char ch[20];
    find_next_word(ch);

    if (verify_if_number(ch)==true)
    {
        success=true;
        num=atoi(ch);
    }

    else
    {
        success=false;
        num=-1;
    }
}

bool Scorpio::WordFinder::verify_if_number(char* c)
{
    for (unsigned int i=0; i<=strlen(c)-1; i++)
    {
        if (c[i]<'0' || c[i]>'9')
        {
            return false;
        }
    }

    return true;
}

Scorpio::WordFinder::~WordFinder()
{
    if (is_valid==true)
    {
        delete[] string_handle;
    }
}

