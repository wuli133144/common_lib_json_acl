#include "json/value.h"
#include "json/reader.h"
#include "json/writer.h"

using namespace std;
using namespace Json;

    
int main(){    
    Value root;
    Reader reader;
    bool status=reader.parse(NULL, root);
    printf("return=%d",status);
    return 0;
}


