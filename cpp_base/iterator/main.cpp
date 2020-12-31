#include <algorithm>
#include "mylist.h"

int main()
{
    List<int> mylist;
    for (int i = 0; i < 5; i++)
    {
        mylist.insert_front(i);
        mylist.insert_end(i+2);
    }
    mylist.display();

    ListIter<ListItem<int> > begin(mylist.front());
    ListIter<ListItem<int> > end;
    ListIter<ListItem<int> > iter;

    return 0;
}