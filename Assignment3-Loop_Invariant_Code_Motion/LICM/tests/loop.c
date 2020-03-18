int loop(int a, int b, int c)
{
        int i, ret = 0;

        do {
                c = b + 2;
                i++;
        } while (i < a);

        return ret + c;
}
