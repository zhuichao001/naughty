
//error:  ‘double’ is not a valid type for a template non-type parameter
template <double f>
double multiply(const double b){
    return b*f;
}

double main(){
    double len = multiply<3>(2*5);
    return 0;
}
