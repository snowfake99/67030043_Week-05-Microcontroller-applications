#include <stdio.h>
#include <string.h>
#include <esp_system.h>
#include <esp_heap_caps.h>
#include <esp_attr.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// Global variables in different memory sections
DRAM_ATTR static char sram_buffer[1024];  // Explicitly place in DRAM using ESP32 attribute
static const char flash_string[] = "Hello from Flash Memory!";  // This will be in .rodata (Flash) automatically
static char *heap_ptr;

// Function to display memory information
void print_memory_info() {
    printf("\n=== ESP32 Memory Layout Analysis ===\n");
    
    // Stack variables (in SRAM)
    int stack_var = 42;
    printf("Stack variable address: 0x%08lx\n", (unsigned long)&stack_var);
    
    // Global SRAM buffer
    printf("SRAM buffer address:    0x%08lx\n", (unsigned long)sram_buffer);
    
    // Flash constant string
    printf("Flash string address:   0x%08lx\n", (unsigned long)flash_string);
    
    // Heap allocation
    heap_ptr = malloc(512);
    printf("Heap allocation:        0x%08lx\n", (unsigned long)heap_ptr);
    
    // Heap information
    printf("\n=== Heap Information ===\n");
    printf("Free heap size:         %lu bytes\n", (unsigned long)esp_get_free_heap_size());
    printf("Min free heap size:     %lu bytes\n", (unsigned long)esp_get_minimum_free_heap_size());
    printf("Largest free block:     %lu bytes\n", (unsigned long)heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));
    
    // Memory usage by type
    printf("\n=== Memory Usage by Type ===\n");
    printf("Internal SRAM:          %lu bytes\n", (unsigned long)heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
    printf("SPI RAM (if available): %lu bytes\n", (unsigned long)heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
    printf("DMA capable memory:     %lu bytes\n", (unsigned long)heap_caps_get_free_size(MALLOC_CAP_DMA));
    
    free(heap_ptr);
}

void app_main() {
    // เพิ่ม delay เล็กน้อยเพื่อให้ serial port พร้อม
    vTaskDelay(100 / portTICK_PERIOD_MS);
    
    printf("=== ESP32 STARTING UP ===\n");
    printf("ESP32 Memory Architecture Analysis\n");
    printf("==================================\n");
    
    // Flush output buffer
    fflush(stdout);
    
    // Test memory operations
    strcpy(sram_buffer, "SRAM Test Data");
    printf("Flash string: %s\n", flash_string);
    printf("SRAM buffer: %s\n", sram_buffer);
    
    fflush(stdout);
    
    print_memory_info();
    
    printf("\nMemory analysis complete!\n");
    fflush(stdout);
}