# cmake_minimum_required(VERSION 3.19)        
# include(pico_sdk_import.cmake)
# project(pico_pio_pwm C CXX ASM)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
# pico_sdk_init()

# add_executable(pio_pwm)
# # pico_generate_pio_header(pio_pwm ${CMAKE_CURRENT_LIST_DIR}/pico_servo.pio)

# target_sources(pio_pwm PRIVATE pio_pwm.c)

# target_link_libraries(pio_pwm PRIVATE
#   pico_stdlib
#   hardware_pio
#   pico_stdlib 
#   hardware_pwm
# )

# # enable usb output, disable uart output
# pico_enable_stdio_usb(pio_pwm 1)
# pico_enable_stdio_uart(pio_pwm 0)

# pico_add_extra_outputs(pio_pwm)

cmake_minimum_required(VERSION 3.19)

# initalize pico_sdk from installed location
# # (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "~/pico/pico-sdk")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_pio_pwm C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()
add_executable(pico_pio_pwm)

pico_generate_pio_header(pico_pio_pwm ${CMAKE_CURRENT_LIST_DIR}/pico_pio_pwm.pio)

target_sources(pico_pio_pwm PRIVATE pico_pio_pwm.c)

target_link_libraries(pico_pio_pwm PRIVATE
  pico_stdlib
  hardware_pio
  pico_stdlib 
  hardware_pwm
)

# enable usb output, disable uart output
pico_enable_stdio_usb(pico_pio_pwm 1)
pico_enable_stdio_uart(pico_pio_pwm 0)

pico_add_extra_outputs(pico_pio_pwm)
