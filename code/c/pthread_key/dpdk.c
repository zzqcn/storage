#include <rte_eal.h>
#include <rte_lcore.h>
#include <rte_debug.h>
#include <unistd.h>

pthread_key_t g_key;

static int lcore_func(__attribute__((unused)) void* arg)
{
    int n, *p;
    
    n = rand() % 100;
    p = NULL;
    printf("lcore %u set: %d\n", rte_lcore_id(), n);
    pthread_setspecific(g_key, &n);
    usleep(100);
    p = pthread_getspecific(g_key);
    printf("lcore %u get: %d\n", rte_lcore_id(), *p);

    return 0;
}

int main(int argc, char** argv)
{
    int ret;
    unsigned lcore_id;

    srand(time(NULL));
    pthread_key_create(&g_key, NULL);

    ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_panic("Cannot init EAL\n");

    RTE_LCORE_FOREACH(lcore_id)
    {
        rte_eal_mp_remote_launch(lcore_func, NULL, CALL_MAIN);
    }

    rte_eal_mp_wait_lcore();

    return 0;
}
