#include<list>
#include<string>
using namespace std;
std::list<std::string> split_utf8_string(const std::string& text)
{
    std::list<std::string> temp;

    do 
    {
        if (text.length() <= 0)
            break;

        std::string::const_iterator begin = text.begin();
        std::string::const_iterator end   = text.end();
        while (begin != end)
        {
            unsigned char c = *begin;
            int n = 0;

            if ((c & 0x80) == 0)    
                n = 1;
            else if ((c & 0xE0) == 0xC0) 
                n = 2;
            else if ((c & 0xF0) == 0xE0) 
                n = 3;
            else if ((c & 0xF8) == 0xF0) 
                n = 4;
            else if ((c & 0xFC) == 0xF8) 
                n = 5;
            else if ((c & 0xFE) == 0xFC) 
                n = 6;
            else 
                break;

            if (end - begin < n) 
                break;

            std::string substring;
            substring += *begin;

            bool isError = false;
            for (int i=1; i<n; ++i)
            {
                if ((begin[i] & 0xC0) != 0x80) 
                {
                    isError = true;
                    break;
                }

                substring += begin[i];
            }

            if (isError)
                break;

            temp.push_back(substring);
            begin += n;
        }
    }while (false);
    return temp;
}