double cylinderVolume(double circumference, double height) {
    return (circumference * circumference * height) / (4 * Pi);
}
    
double idealJugVolume () {
    double coin1 = cylinderVolume(117.0, 25.0);
    double coin2 = cylinderVolume(117.0, 25.0);
    double coin3 = cylinderVolume(117.0, 25.0);
    double coin4 = cylinderVolume(117.0, 25.0);
    return coin1 + coin2 + coin3 + coin4; 
}

double actualJugVolume () {
    double coin1 = cylinderVolume(35.3, 1.8);
    double coin2 = cylinderVolume(34.2, 2.1);
    double coin3 = cylinderVolume(32.8, 2.8);
    double coin4 = cylinderVolume(30.2, 3.5);
    return coin1 + coin2 + coin3 + coin4; 
}