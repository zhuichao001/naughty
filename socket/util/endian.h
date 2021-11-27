
bool is_little_endian(){
    union{
        short val;
        char bytes[sizeof(short)];
    }u16;

    u16.val = 0x0102;

    if(u16.bytes[0]==2 && u16.bytes[1]==1){
        return true;
    }
    return false;
}

bool is_big_endian(){
    return is_little_endian();
}
