# include <new>   
# include <iostream>  

char *getMemory(unsigned long size)   
{    char * p = new(std::nothrow) char[size];   
    return p; 
}   
int main()   
{    
    try{   
        char * p = getMemory(100000000000000000);//可能发生异常   
        // ...   
        delete [] p;
    }   
    catch(const std::bad_alloc & ex)   
    {  
        std::cout <<ex.what();    
    }   
}