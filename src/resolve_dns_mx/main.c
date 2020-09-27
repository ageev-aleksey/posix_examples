#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <resolv.h>
#include <netdb.h>

#define N 4096

int main (int argc, char *argv[])
{
    u_char nsbuf[N];
    char dispbuf[N];
    ns_msg msg;
    ns_rr rr;
    int i, l;

    if (argc < 2) {
        printf ("Usage: %s <domain>\n", argv[0]);
        exit (1);
    }

    // HEADER
    printf("Domain : %s\n", argv[1]);
    // ------
    // A RECORD
    printf("A records : \n");
    l = res_query(argv[1], ns_c_in, ns_t_a, nsbuf, sizeof(nsbuf));
    if (l < 0)
    {
        perror(argv[1]);
    } else {
        ns_initparse(nsbuf, l, &msg);
        l = ns_msg_count(msg, ns_s_an);
        for (i = 0; i < l; i++)
        {
            ns_parserr(&msg, ns_s_an, i, &rr);
            ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
            printf("\t%s \n", dispbuf);
        }
    }

    //------------


    // MX RECORD
    printf("MX records : \n");
    l = res_query(argv[1], ns_c_in, ns_t_mx, nsbuf, sizeof(nsbuf));
    if (l < 0)
    {
        perror(argv[1]);
    }
    else
    {
#ifdef USE_PQUERY
        /* this will give lots of detailed info on the request and reply */
      res_pquery(&_res, nsbuf, l, stdout);
#else
        /* just grab the MX answer info */
        ns_initparse(nsbuf, l, &msg);
        l = ns_msg_count(msg, ns_s_an);
        for (i = 0; i < l; i++)
        {
            ns_parserr(&msg, ns_s_an, i, &rr);
            ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
            printf ("\t%s\n", dispbuf);
        }
#endif
    }
    // ---------

    // NS RECORD
    printf("NS records : \n");
    l = res_query(argv[1], ns_c_in, ns_t_ns, nsbuf, sizeof(nsbuf));
    if (l < 0)
    {
        perror(argv[1]);
    } else {
        ns_initparse(nsbuf, l, &msg);
        l = ns_msg_count(msg, ns_s_an);
        for (i = 0; i < l; i++)
        {
            ns_parserr(&msg, ns_s_an, i, &rr);
            ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
            printf("\t%s \n", dispbuf);
        }
    }

    // ---------


    res_init();
    // PTR RECORD
    printf("PTR records : \n");
    l = res_query(argv[1], C_IN, T_PTR, nsbuf, sizeof(nsbuf));
    if (l < 0)
    {
        perror(argv[1]);
    }
    else
    {
#ifdef USE_PQUERY
        /* this will give lots of detailed info on the request and reply */
      res_pquery(&_res, nsbuf, l, stdout);
#else
        /* just grab the MX answer info */
        ns_initparse(nsbuf, l, &msg);
        l = ns_msg_count(msg, ns_s_an);
        for (i = 0; i < l; i++)
        {
            ns_parserr(&msg, ns_s_an, i, &rr);
            ns_sprintrr(&msg, &rr, NULL, NULL, dispbuf, sizeof(dispbuf));
            printf ("\t%s\n", dispbuf);
        }
#endif
    }
    return 0;
}