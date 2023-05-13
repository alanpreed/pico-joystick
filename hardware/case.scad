// Modules for generating the casework for the Pico Joystick adapter
//
// Copyright Alan Reed 2023 (areed.me)

include <dsub.scad>;
include <board.scad>;
include <utilities.scad>;

// Lower PCB support pillars
case_lower_pillar_z = 4;
case_lower_pillar_r = 5;

// Lid pillars
case_upper_pillar_r = 4;
case_upper_pillar_clearance_z = 0.2;

// Threaded inserts in lid pillars
case_threaded_insert_d = 3.7;
case_threaded_insert_z = 8;

// Internal volume dimensions
case_inner_x = pcb_x + 2;
case_inner_y = pcb_y + (dsub_body_y - pcb_dsub_y_offset) + 1;
case_inner_z = pcb_z + dsub_body_z + case_lower_pillar_z + 4;
case_corner_radius = 2;
case_wall_thickness = 2;

// USB connnector hole
case_usb_clearance = 2;
case_usb_hole_x = pico_usb_x + case_usb_clearance;
case_usb_hole_z = pico_usb_z + case_usb_clearance;

// Case assembly bolt holes
case_bolt_hole_d = pcb_hole_diameter;
case_countersink_depth = 2.25;

// Case lid features
case_lid_insert_depth = 1.5;
case_lid_leeway = 0.2;
case_text_size = 5;
case_text_depth = 0.8;
case_text_separation = 30;

case_dsub_hole_d = 5.5;


// Moves child to the same position as the PCB will be fitted
module case_pcb_position() {
    translate([0, (dsub_body_y - pcb_dsub_y_offset) / 2, case_lower_pillar_z - (case_inner_z - pcb_z) / 2]){
        children();
    }
}

// Main casework body component
module case_body() {
    difference() {
        union(){
            // Main case walls and base
            difference() {
                // Body height doesn't include lid thickness, so shift down to keep centred
                translate([0, 0, -case_wall_thickness / 2]) {
                    rounded_rectangle(case_inner_x + 2 * case_wall_thickness, case_inner_y + 2 * case_wall_thickness, case_inner_z + case_wall_thickness, case_corner_radius);
                }
                translate([0, 0, case_inner_z / 2]) { 
                    rounded_rectangle(case_inner_x, case_inner_y, case_inner_z * 2, case_corner_radius);
                }
            }

            // Mounting pillars
            case_pcb_position(){
                board_hole_positions() {
                    translate([0, 0, -(case_lower_pillar_z + pcb_z) / 2]){
                        cylinder(case_lower_pillar_z, case_lower_pillar_r, case_lower_pillar_r, $fn=50, center = true);
                    }
                }
            }
        }
        
        // Countersunk assembly holes
        case_pcb_position(){
            board_hole_positions() {
                translate([0, 0, -((pcb_z / 2) + case_lower_pillar_z + case_wall_thickness)]){
                    countersunk_bolt_hole(case_bolt_hole_d,
                                          case_countersink_depth,
                                          2 * (case_wall_thickness + case_lower_pillar_z));
                }
            }
        }
        
        // DSub entry hole
        hole_depth = case_wall_thickness * 2;
        
        translate([0, - ((pcb_y + hole_depth) / 2 + (dsub_body_y - pcb_dsub_y_offset)), dsub_plug_offset_z + (pcb_z / 2)]){
            case_pcb_position(){
                rotate([90, 0, 0]){
                    dsub_hole(hole_depth, mount_hole_diameter = case_dsub_hole_d);
                }
            }
        }
        
        // USB entry hole
        case_pcb_position() {
            board_pico_position(){
                pico_usb(){
                    translate([0, 0, (pico_z + pico_usb_z) / 2]){
                        cube([case_usb_hole_x, 2 * pico_y, case_usb_hole_z], center=true);
                    }
                }
            }
        }
    }
}

// Main casework lid component
module case_lid() {
    translate([0, 0, (case_inner_z + case_wall_thickness) / 2]){
        union() {
            // Outer surface
            difference() {
                rounded_rectangle(case_inner_x + 2 * case_wall_thickness,
                                  case_inner_y + 2 * case_wall_thickness,
                                  case_wall_thickness,
                                  case_corner_radius);
                
                // Lid label text
                translate([0, 0, case_wall_thickness / 2 - case_text_depth]){
                    
                    translate([0, case_text_separation / 2, 0]){
                        linear_extrude(case_text_depth + 0.001){ // Small extra prevents artifacts
                            text("Pico Joystick", font = "Noto Sans:style=Bold", halign="center", size=case_text_size);
                        }
                    }
                    translate([0, -case_text_separation / 2, 0]){
                        linear_extrude(case_text_depth + 0.001){ // Small extra prevents artifacts
                            text("Alan Reed", font = "Noto Sans:style=Bold", halign="center", size=case_text_size);
                        }
                    }
                }
            }
            
            // Mounting pillars
            upper_pillar_z = case_inner_z - case_lower_pillar_z - pcb_z - case_upper_pillar_clearance_z;
            case_pcb_position(){
                board_hole_positions() {
                    translate([0, 0, -(case_wall_thickness + upper_pillar_z) / 2 - (case_lower_pillar_z - (case_inner_z - pcb_z) / 2)]){
                        difference(){
                            cylinder(h = upper_pillar_z, r = case_upper_pillar_r, $fn=50, center = true);
                            
                            // Threaded insert holes
                            translate([0, 0, -(upper_pillar_z / 2)]){
                                cylinder(h = case_threaded_insert_z * 2, r = case_threaded_insert_d / 2, $fn = 50, center=true);
                            }
                        }
                    }
                }
            }
            
            // Inner ridge
            translate([0, 0, -(case_wall_thickness + case_lid_insert_depth) / 2]) {
                difference() {
                    rounded_rectangle(case_inner_x - case_lid_leeway,
                                      case_inner_y - case_lid_leeway,
                                      case_lid_insert_depth, case_corner_radius);
                    
                    rounded_rectangle(case_inner_x - 2 * case_wall_thickness - case_lid_leeway,
                                      case_inner_y - 2 * case_wall_thickness - case_lid_leeway,
                                      2 * case_lid_insert_depth, case_corner_radius);
        
                }
            }
        }
    }
}


// Display exploded view of case, PCB and lid
case_body();

translate([0, 0, 40]){
    case_pcb_position(){
        color("green") {
            board_volume();
        }    
    }
}

translate([0, 0, 70]){
    case_lid();
}
