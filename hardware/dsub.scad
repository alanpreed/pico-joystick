// Modules for D-Subminature connector volume model and mounting hole generation
// Default dimensions are for a DS-15 connector
//
// Copyright Alan Reed 2023 (areed.me)

// Socket hole dimensions
dsub_plug_width = 27;
dsub_plug_height = 8.36;
dsub_mount_hole_separation = 33.32;

// Socket body dimensions
dsub_body_x = 39.1;
dsub_body_y = 20.6 - 5.84;
dsub_body_z = 12.55;


// Generates three volumes which correspond to the DSub shell and mounting holes
module dsub_hole(depth, mount_hole_diameter = 3.5, plug_side_angle = 10, corner_radius = 2.33) {

    y_offset = (dsub_plug_height - 2 * corner_radius) * tan(plug_side_angle);

    plug_x_coord_upper = (dsub_plug_width / 2) - corner_radius;
    plug_x_coord_lower = plug_x_coord_upper - y_offset;
    plug_y_coord = (dsub_plug_height / 2) -  corner_radius;

    corner_positions = [[plug_x_coord_upper, plug_y_coord, 0],
                        [-plug_x_coord_upper, plug_y_coord, 0],
                        [plug_x_coord_lower, -plug_y_coord, 0],
                        [-plug_x_coord_lower, -plug_y_coord, 0]];

    hull(){
        for( position = corner_positions) {
            translate(position){
                cylinder(depth, corner_radius, corner_radius, center=true, $fn=500);
            }
        }
    }

    for (x = [-dsub_mount_hole_separation / 2, dsub_mount_hole_separation / 2]) {
        translate([x,0,0]){
            cylinder(depth, mount_hole_diameter/2, mount_hole_diameter/2, center=true, $fn=500);
        }
    }
}

// Generates a rough volume model of a 90deg PCB mount DSub socket
module dsub_volume(dsub_hole_depth = 5.84) {
    union(){
        cube([dsub_body_x, dsub_body_y, dsub_body_z], center = true);
        
        translate([0, (dsub_body_y + dsub_hole_depth) / 2, 0]){
            rotate([90, 0, 0]){
                dsub_hole(dsub_hole_depth);
            }
        }
    }
}
