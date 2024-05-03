int main(int argc, char *argv[])
{
  int a=10;
  int *p = &a;
  printf("*p = %d\n", p);
  printf("*p = %d\n", &a);
}
