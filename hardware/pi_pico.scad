// Modules for Raspberry Pi Pico volume model and mounting hole generation
// 
// Copyright 2023 Alan Reed (areed.me)

pico_x = 21;
pico_y = 51;
pico_z = 1;
pico_hole_d = 2.1;
pico_hole_x_pos = 11.4 / 2;
pico_hole_y_pos = (pico_y / 2) - 2;

pico_button_x = 3.4;
pico_button_y = 4.3;
pico_button_z = 2.5;
pico_button_x_pos = -((pico_x / 2) - 5.5);
pico_button_y_pos = ((pico_y / 2) - 14.5);

pico_led_x = 2;
pico_led_y = 1;
pico_led_z = 0.5;
pico_led_x_pos = -(pico_x / 2 - 3.75);
pico_led_y_pos = pico_y / 2 - 5;

pico_usb_x = 7.6;
pico_usb_y = 5.5;
pico_usb_z = 2.8;
pico_usb_x_pos = 0;
pico_usb_y_pos = pico_y/2 - 4.5;
pico_usb_z_pos = 0;

// Locates child objects at the Pi Pico's mounting hole centres
module pico_holes(){
    for (i = [[pico_hole_x_pos, pico_hole_y_pos, 0], 
           [pico_hole_x_pos, -pico_hole_y_pos, 0], 
           [-pico_hole_x_pos, -pico_hole_y_pos, 0], 
           [-pico_hole_x_pos, pico_hole_y_pos, 0]]){
        translate(i){
            children();
        }
    }
}

// Locates child objects at the Pi Pico's button position
module pico_button(){
    translate([pico_button_x_pos + pico_button_x / 2, pico_button_y_pos + pico_button_y / 2, 0]){
        children();
    }
}

// Locates child objects at the Pi Pico's LED position
module pico_led(){
    translate([pico_led_x_pos + pico_led_x / 2, pico_led_y_pos + pico_led_y / 2, 0]){
        children();
    }
}

// Locates child objects at the Pi Pico's USB socket position
module pico_usb(){
    translate([pico_usb_x_pos, pico_usb_y_pos + pico_usb_y / 2, pico_usb_z_pos]){
        children();
    }
}

// Rough volume model of the Raspberry Pi Pico
module pico(){
    difference(){
        union(){
            cube([pico_x, pico_y, pico_z], center=true);
            translate([0, 0, pico_z / 2]){
                pico_button(){
                    translate([0, 0, pico_button_z / 2]){
                        cube([pico_button_x, pico_button_y, pico_button_z], center=true);
                    }
                }
                pico_led(){
                    translate([0, 0, pico_led_z / 2]){
                        cube([pico_led_x, pico_led_y, pico_led_z], center=true);
                    }
                }
                pico_usb(){
                    translate([0, 0, pico_usb_z / 2]){
                        cube([pico_usb_x, pico_usb_y, pico_usb_z], center=true);
                    }
                }                
            }
        }
        
        pico_holes(){
            cylinder(h = 2 * pico_z, d = pico_hole_d, center=true, $fn=20);
        }
    }
}
