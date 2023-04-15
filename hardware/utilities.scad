// General modules useful for casework modelling
//
// Copyright Alan Reed 2023 (areed.me)

// Replicate children at the corners of a rectangle of the specified size
module corners(x_size, y_size){
    for(p = [[x_size/2, y_size/2, 0],
            [-x_size/2, y_size/2, 0],
            [-x_size/2, -y_size/2, 0],
            [x_size/2, -y_size/2, 0]]) {
        translate(p){
            children();
        }
    }
}

//  Generate a rounded rectangle with the specified corner radius
module rounded_rectangle(x, y, z, radius){
    union(){
        cube([x - 2 * radius, y, z], center=true);
        cube([x, y - 2 * radius, z], center=true);

        corners(x - 2 * radius, y - 2 * radius){
            cylinder(r = radius, h = z, center=true, $fn=30);
        }
    }
}

// Generate an inverse of a countersunk hole, for subtraction from a volume
// Top surface of countersink is centred on plane z = 0
// Countersink angle of 90 is metric standard (https://en.wikipedia.org/wiki/Countersink)
// countersink_pad includes a tiny amount of extra countersinking to prevent graphical artifacts during difference
module countersunk_bolt_hole(diameter, countersink_depth, hole_depth,
                             countersink_angle = 90, countersink_inset = 1,
                             countersink_pad = 0.001, $fn = 50){
    
    countersink_padded_depth = countersink_depth + countersink_pad;
    countersink_extra_r = countersink_padded_depth * tan(countersink_angle / 2);
    
    echo("Countersink face diameter: ", diameter + 2 *countersink_extra_r);
    
    union(){
        rotate_extrude(angle=360, $fn=$fn){
            polygon([[0, -countersink_pad], 
                     [0, countersink_depth],
                     [diameter / 2, countersink_depth], 
                     [diameter / 2 + countersink_extra_r, -countersink_pad]]);
        }
        translate([0, 0, countersink_depth/2]){
            cylinder(r=diameter/2, h=hole_depth, $fn=$fn);   
        }
        
        translate([0, 0, -(countersink_inset + countersink_pad)]){
            cylinder(r=diameter / 2 + countersink_extra_r, h=countersink_inset, $fn=$fn);   
        }
    }
}
