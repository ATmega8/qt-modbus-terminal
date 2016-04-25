#include "ekf_data.h"

#include <iostream>
#include <cassert>

EKFData::EKFData(int stateVariableCount, int observationVariableCount)
{
	m_stateVariable.resize(stateVariableCount);

	m_observationNoise.resize(observationVariableCount);

	m_processNoise.resize(stateVariableCount);

	m_observationMatrix.resize(stateVariableCount*observationVariableCount);

	m_covarianceMatrix.resize(stateVariableCount*stateVariableCount);
}

EKFData::~EKFData(void)
{

}

void EKFData::setInitialStateVariable(const vector<double> stateVariable)
{
	assert(stateVariable.size() == m_stateVariable.size());

	m_stateVariable = stateVariable;
}

void EKFData::setInitialCovarianceValue(const double cov)
{
	unsigned int i;

	m_covarianceMatrix.resize(m_stateVariable.size()*m_stateVariable.size());


	for(i = 0; i < m_covarianceMatrix.size(); i++)
		m_covarianceMatrix[i] = cov;
}

void EKFData::setObservationNoiseValue(const vector<double> observationNoise)
{
	assert(observationNoise.size() == m_observationNoise.size());
	m_observationNoise = observationNoise;
}

void EKFData::setProcessNoiseValue(const vector<double> processNoise)
{
	assert(processNoise.size() == m_processNoise.size());
	m_processNoise = processNoise;
}

void EKFData::setObservationMatrix(const vector<double> observationMatrix)
{
	assert(observationMatrix.size() == m_observationMatrix.size());
	m_observationMatrix = observationMatrix;
}

void EKFData::setStateMatrix(const vector<double> stateMatrix)
{
	assert(stateMatrix.size() == m_stateMatrix.size());
	m_stateMatrix = stateMatrix;
}

vector<double> EKFData::stateVariable(void) const
{
	return m_stateVariable;
}

vector<double> EKFData::observationNoiseValue(void) const
{
	return m_observationNoise;
}

vector<double> EKFData::processNoiseValue(void) const
{
	return m_processNoise;
}

vector<double> EKFData::observationMatrix(void) const
{
	return m_observationMatrix;
}

vector<double> EKFData::covarianceMatrix(void) const
{
	return m_covarianceMatrix;
}


