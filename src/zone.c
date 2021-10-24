#include "../includes/memory_handler.h"

t_block *ft_find_the_optimal_free_block_in_zone(size_t size, t_zone *zone) {
    if (zone) {
        t_block *block = zone->blocks;
        t_block *block_best = NULL;

        while (block) {
            if (block->free && size <= block->size && (!block_best || size < block_best->size))
                block_best = block;
            block = block->next;
        }

        return block_best;
    }

    return NULL;
}

t_block *ft_find_the_optimal_free_block_in_zones(size_t size, t_zone *zone) {
    if (zone) {
        t_block *block = NULL;
        
        while (!(block = ft_find_the_optimal_free_block_in_zone(size, zone)) && zone) {
            zone = zone->next;
        }

        return block;
    }

    return NULL;
}


// t_zone *ft_find_zone_from_a_pointer_in_specific_zone_type(void *ptr, t_zone *zone) {
//     while (zone && ZONE_DATA_START_TINY(zone) >= ptr)
// }

t_zone *ft_find_zone_from_a_pointer(void *ptr) {
    t_zone *zone = g_tiny_first_zone;
    printf("******************************\n");
    printf("******************************\n");
    printf("******************************\n");

    printf("zone = %p\n", zone);
    printf("zone = %p\n", ZONE_DATA_START_TINY(zone));
    printf("zone = %p\n", ZONE_DATA_END_TINY(zone));
}

t_zone *ft_create_zone(t_zone *previous_zone, int zone_size) {
    t_zone *zone = (t_zone *)mmap(0, zone_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);

    printf("********** ft_create_zone ********** [%p] size = %d\n", zone, zone_size);

    zone->blocks = (t_block *)(zone + 1);
    zone->blocks->free = true;
    zone->blocks->size = zone_size - STRUCT_ZONE_SIZE - STRUCT_BLOCK_SIZE;
    zone->blocks->previous = NULL;
    zone->blocks->next = NULL;
    zone->previous = previous_zone;
    zone->next = NULL;

    return zone;
}

void ft_show_zone(t_zone *zone) {
    printf(">>>>>>>>>>>>>>>> zone %p\n", zone);

    if (zone) {
        t_block *ptr;
        ptr = zone->blocks;
        while (ptr) {
            printf("----------------------------------------------\n");
            printf("prev : %p%s\t free : %s\n", ptr->previous, ptr->previous ? "" : "\t", ptr->free ? "true" : "false");
            printf("curr : %p%s\t size : %ld\tbytes\n", ptr, ptr ? "" : "\t", ptr->size);
            printf("next : %p\n", ptr->next);
            ptr = ptr->next;
        }
    } else {
        printf("****** zone is nil\n");
    }
}

void ft_show_zones() {
    printf("\n===============================================\n");
    printf("================ ft_show_zones ================\n");
    printf("===============================================\n");
    t_zone *zone;
    printf("\n================== tiny_zones =================\n");
    zone = g_tiny_first_zone;
    while (zone) {
        ft_show_zone(zone);
        zone = zone->next;
    }
    printf("\n================= small_zones =================\n");
    zone = g_small_first_zone;
    while (zone) {
        ft_show_zone(zone);
        zone = zone->next;
    }
}