#ifndef MOTIONFLAPPING_H
#define MOTIONFLAPPING_H

#include "imotion.h"
namespace sdfibm {

class MotionFlapping:public IMotion, _creator<MotionFlapping>
{
public:
    // same signature for all motions
    virtual void constraint(
            const scalar& time,
            vector& velocity,
            vector& omega) override final;

    // update below
    MotionFlapping(const dictionary& para)
    {
        try {
            m_plunging_amplitude = Foam::readScalar(para.lookup("plunging_amplitude")); // in m
            m_angular_amplitude = Foam::readScalar(para.lookup("angular_amplitude"));   // in degree
            m_angular_amplitude = m_angular_amplitude * M_PI/180.0; // in rad
            m_period    = Foam::readScalar(para.lookup("period"));                      // in s
            m_omega = 2*M_PI/m_period;

        } catch (const std::exception& e) {
            std::cout << "Problem in creating Flapping Motion: "
                      << e.what() << std::endl;
        }
    }
    virtual ~MotionFlapping() override final {}
    TYPENAME("MotionFlapping")
    virtual std::string description() const override {return "harmonic flapping motion";}
private:
    scalar m_plunging_amplitude, m_angular_amplitude, m_period, m_omega;
};

void MotionFlapping::constraint(const scalar &time, vector &velocity, vector &omega)
{
    velocity = vector::zero;
    omega = vector::zero;
    velocity.x() = m_plunging_amplitude * m_omega * std::sin(m_omega*time);
    omega.z() = - m_angular_amplitude * m_omega * std::cos(m_omega*time);
}

}
#endif // MOTIONFLAPPING_H
