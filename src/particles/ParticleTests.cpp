/*
 *  Particles test file for Nuke.
 *
 *  Copyright (C) 2015 Rob Clucas robclu1818@gmail.com
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published
 *  by the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation,
 *	Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <gtest/gtest.h>

#include "Particles.hpp"
#include <iostream>

// Define a test class for the particles
class ParticleTest : public ::testing::Test
{
protected :
	Particles<1, Particle, ParticleInitializer, ParticleAction> ParticlesSmall;
	Particles<100, Particle, ParticleInitializer, ParticleAction> ParticlesMed;
	Particles<1000, Particle, ParticleInitializer, ParticleAction> ParticlesLarge;
	Particles<10000, Particle, ParticleInitializer, ParticleAction> ParticlesHuge;
};
	

TEST_F(ParticleTest, ParticlesAreNotCreatedOnConstruction)
{
	// Get a pointer to the particles. Since none have been emitted this 
	// should be a NULL pointer
	const Particle* testParticles = ParticlesLarge.GetParticles();

	EXPECT_TRUE(testParticles == NULL);
}

TEST_F(ParticleTest, CanCreateParticleInParticleArray)
{
	// Emit the particle (the particle array is empty when no particles are
	// emitted) at the origin
	ParticlesSmall.Emit(1, vec3(0.f, 0.f, 0.f));

	// Get a pointer to the particle
	const Particle* testParticle = ParticlesSmall.GetParticles();

	EXPECT_TRUE(testParticle !=  NULL);
}

TEST_F(ParticleTest, LifeIsCorrectlyCreated)
{
	// Emit them (Make them active) just off the origin
	ParticlesLarge.Emit(1000, vec3(1.f, 2.f, 3.f));

	// Get a pointer to the particle group 
	const Particle* testParticles = ParticlesLarge.GetParticles();

	// Check that each one has life 
	for (size_t i = 0; i < 1000; i++) {
		EXPECT_GT(testParticles[i].life, 0.f);
		EXPECT_LT(testParticles[i].life, 1.f);
		}
}

TEST_F(ParticleTest, VelocityIsCorrectlyInitialized)
{
	// Emit (make active) 100 particles
	ParticlesMed.Emit(100, vec3(0.0f, 0.0f, 0.0f));

	// Get a pointer to the particles
	const Particle* testParticles = ParticlesMed.GetParticles();

	// Check that their velocities are correct
	for (size_t i = 0; i < 100; i++) {
		EXPECT_EQ(vec3(0.f), testParticles[i].velocity);
	}
}

TEST_F(ParticleTest, VelocityCenBeSetCorrectly)
{
	// Set the velocity for the particles
	ParticlesMed.initializerPolicy.velocityPolicy.SetVelocity(vec3(1.f, 2.f, 3.f));
	
	// Make them active
	ParticlesMed.Emit(100, vec3(0.0f, 0.0f, 0.0f));

	// Get a pointer to the particles
	const Particle* testParticles = ParticlesMed.GetParticles();

	// Check that their velocities are correct
	for (size_t i = 0; i < 100; i++) {
		EXPECT_EQ(vec3(1.f, 2.f, 3.f), testParticles[i].velocity);
	}
}

TEST_F(ParticleTest, ParticlesMoveCorrectlyForVelocityWithNoEffects)
{
	// Set the velocity for the particles
	ParticlesMed.initializerPolicy.velocityPolicy.SetVelocity(vec3(1.f, 2.f, 3.f));
	
	// Make them active at the origin
	ParticlesMed.Emit(100, vec3(0.0f, 0.0f, 0.0f));

	// Update the particles (making the velocity take effect)
	ParticlesMed.Update();

	// Get a pointer to the particles
	const Particle* testParticles = ParticlesMed.GetParticles();

	// Check that the positions have been moved from the origin
	for (size_t i = 0; i < 100; i++) {
		EXPECT_EQ(vec3(1.f, 2.f, 3.f), testParticles[i].pos);
	}
}
int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
