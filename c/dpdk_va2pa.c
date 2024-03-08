
int main(int argc, char *argv[])
{
  uint8_t *p = malloc(1024 * 1024);

  *(p + 4096) = 10;
  printf("virt:%p phys:%p\n", p + 4096, rte_mem_virt2phy(p + 4096));

  *(p + 2 * 4096) = 10;
  printf("virt:%p phys:%p\n", p + 2 * 4096, rte_mem_virt2phy(p + 2 * 4096));
}