#include "TLorentzVector.h"

class lep {

public:
    lep(double pt, TLorentzVector p4);
    double PTrans_;
    TLorentzVector fourmom_;
};

lep::lep(double pt, TLorentzVector p4) {
    PTrans_ = pt;
    fourmom_ = p4;
}
