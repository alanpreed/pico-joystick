// Modules related to the Pico Joystick PCB
//
// Copyright Alan Reed 2023 (areed.me)

include <pi_pico.scad>;
include <dsub.scad>;

// Board dimensions
pcb_x = 50;
pcb_y = 70;
pcb_z = 1.6;

// Inset of PCB holes from edges. Y inset for lower holes is larger due to DSub
pcb_hole_diameter = 3.2; 
pcb_hole_offset = 5;
pcb_socket_hole_offset = 17;

// Distance between DSub back edge and board bottom edge
pcb_dsub_y_offset = 12.48;

// Vertical position of Pi Pico
pcb_pico_mount_height = 2.5;
// Distance between Pico top edge and board top edge
pcb_pico_y_offset = 0.76;

// Locates child objects at the board mounting hole centres
module board_hole_positions() {
    x_coord = (pcb_x / 2) - pcb_hole_offset;
    y_coord_upper = (pcb_y / 2) - pcb_hole_offset;
    y_coord_lower = (pcb_y / 2) - pcb_socket_hole_offset;
    
    hole_positions = [[x_coord, y_coord_upper, 0],
                      [-x_coord, y_coord_upper, 0],
                      [x_coord, -y_coord_lower, 0],
                      [-x_coord, -y_coord_lower, 0]]; 
    
    for (position = hole_positions) {
        translate(position) {
            children();
        }
    }
}

// Locates child objects at the Pi Pico mount position
module board_pico_position(){
    translate([0, ((pcb_y - pico_y) / 2) - pcb_pico_y_offset, ((pico_z + pcb_z) / 2) + pcb_pico_mount_height]){
        children();
    }  
}

// Rough volume model of the Pico Joystick PCB
module board_volume() {
    union() {
        difference() {
            // Main PCB body
            cube([pcb_x, pcb_y, pcb_z], center=true);
            
            // PCB mounting holes
            board_hole_positions() {
                cylinder(pcb_z * 2, pcb_hole_diameter / 2, pcb_hole_diameter / 2, center = true, $fn=50);
            }
            
            // Pico mounting holes
            translate([0, ((pcb_y - pico_y) / 2) - pcb_pico_y_offset, 0]){            
                pico_holes() {
                    cylinder(pcb_z * 2, pico_hole_d / 2, pico_hole_d / 2, center = true, $fn=50);
                }
            }
        }
        
        // Pico volume model
        board_pico_position(){
            pico();
        }
        
        // DSub volume model
        translate([0, pcb_dsub_y_offset - (dsub_body_y + pcb_y) / 2, (dsub_body_z + pcb_z) / 2]){
            rotate([0, 0, 180]) {
                dsub_volume();
            }   
        }
    }
}
