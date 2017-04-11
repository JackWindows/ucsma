#!/usr/bin/python3

import random, os, sys
from collections import OrderedDict

DELTA_100us_PRE_CALCED_MID = OrderedDict([(0, OrderedDict([(0, 101), (1, 115), (3, 142), (7, 193), (15, 282), (31, 449), (63, 772), (127, 1414), (255, 2695), (511, 5256), (1023, 10376)])), (1, OrderedDict([(1, 101), (3, 128), (7, 178), (15, 267), (31, 434), (63, 757), (127, 1399), (255, 2680), (511, 5241), (1023, 10361)])), (3, OrderedDict([(3, 101), (7, 149), (15, 238), (31, 404), (63, 727), (127, 1369), (255, 2650), (511, 5211), (1023, 10331)])), (7, OrderedDict([(7, 101), (15, 185), (31, 347), (63, 669), (127, 1310), (255, 2591), (511, 5151), (1023, 10271)])), (15, OrderedDict([(15, 101), (31, 249), (63, 561), (127, 1196), (255, 2474), (511, 5032), (1023, 10152)])), (31, OrderedDict([(31, 101), (63, 378), (127, 990), (255, 2252), (511, 4802), (1023, 9917)])), (63, OrderedDict([(63, 101), (127, 637), (255, 1849), (511, 4365), (1023, 9460)])), (127, OrderedDict([(127, 101), (255, 1157), (511, 3569), (1023, 8593)])), (255, OrderedDict([(255, 101), (511, 2199), (1023, 7009)])), (511, OrderedDict([(511, 101), (1023, 4282)])), (1023, OrderedDict([(1023, 101)]))])

def CSMA_arbiter_discrete(CWmid, CWedge):
	'''
		return True if mid node accquires the channel
	'''
	backoff_mid = random.randint(0, CWmid)
	backoff_edge1 = random.randint(0, CWedge)
	backoff_edge2 = random.randint(0, CWedge)
	if backoff_mid <= backoff_edge1 and backoff_mid <= backoff_edge2:
		return True
	else:
		return False

def unlocking_arbiter_discrete(CWmid, CWedge, delta, equal=False):
	'''
		return True if mid node accquires the channel
		note: CW is in the unit of slot (20us), while delta is in the unit of 1us
	'''
	if not equal:
		delta_mid = delta
	else:
		delta_mid = DELTA_100us_PRE_CALCED_MID[CWmid][CWedge]
	backoff_mid = random.randint(0, CWmid)
	backoff_edge1 = random.randint(0, CWedge)
	backoff_edge2 = random.randint(0, CWedge)
	artificial_backoff_mid = random.randint(0, 2 * delta_mid)
	artificial_backoff_edge1 = random.randint(0, 2 * delta)
	artificial_backoff_edge2 = random.randint(0, 2 * delta)
	time_mid = backoff_mid * 20 + artificial_backoff_mid
	time_edge1 = backoff_edge1 * 20 + artificial_backoff_edge1
	time_edge2 = backoff_edge2 * 20 + artificial_backoff_edge2
	if time_mid <= time_edge1 and time_mid <= time_edge2:
		return True
	else:
		return False

def CSMA_arbiter_continuous(CWmid, CWedge):
	'''
		return True if mid node accquires the channel
		note: CW is in the unit of 1us
	'''
	backoff_mid = random.uniform(0, CWmid)
	backoff_edge1 = random.uniform(0, CWedge)
	backoff_edge2 = random.uniform(0, CWedge)
	if backoff_mid <= backoff_edge1 and backoff_mid <= backoff_edge2:
		return True
	else:
		return False

def unlocking_arbiter_continuous(CWmid, CWedge, delta):
	'''
		return True if mid node accquires the channel
		note: CW and delta are both in the unit of 1us
	'''
	backoff_mid = random.uniform(0, CWmid)
	backoff_edge1 = random.uniform(0, CWedge)
	backoff_edge2 = random.uniform(0, CWedge)
	artificial_backoff_mid = random.uniform(0, 2 * delta)
	artificial_backoff_edge1 = random.uniform(0, 2 * delta)
	artificial_backoff_edge2 = random.uniform(0, 2 * delta)
	time_mid = backoff_mid + artificial_backoff_mid
	time_edge1 = backoff_edge1 + artificial_backoff_edge1
	time_edge2 = backoff_edge2 + artificial_backoff_edge2
	if time_mid <= time_edge1 and time_mid <= time_edge2:
		return True
	else:
		return False

def simulate_discrete(T, c, CWmid, CWedge, delta, equal):
	'''
		T: unlocking period
		c: how many cycles to simulate
		CWmid, CWedge: in unit of slot time(20us)
		delta: in unit of 1us
	'''
	num_accquired = 0
	num_slots = T * c
	for rounds in range(c):
		if unlocking_arbiter_discrete(CWmid, CWedge, delta, equal):
			num_accquired += 1
		else:
			continue
		for i in range(1, T):
			if CSMA_arbiter_discrete(CWmid, CWedge):
				num_accquired += 1
			else:
				break
	portion_accquired = num_accquired / num_slots * 100
	print("Discrete Simulation: accquired %d out of %d (%.2f%%) slots in %d rounds" \
			% (num_accquired, num_slots, portion_accquired, c))
	return

def test_probability_discrete(CWmid, CWedge, delta, equal=False):
	count = 100000
	num_accquired_P1 = 0
	for rounds in range(count):
		if CSMA_arbiter_discrete(CWmid, CWedge):
			num_accquired_P1 += 1
	num_accquired_P2 = 0
	for rounds in range(count):
		if unlocking_arbiter_discrete(CWmid, CWedge, delta, equal):
			num_accquired_P2 += 1
	sP1 = num_accquired_P1 / count * 100
	sP2 = num_accquired_P2 / count * 100
	print("simultated P1=%.2f%% P2=%.2f%%" % (sP1, sP2))
	

def simulate_continuous(T, c, CWmid, CWedge, delta):
	'''
		T: unlocking period
		c: how many cycles to simulate
		CWmid, CWedge: in unit of 1us
		delta: in unit of 1us
	'''
	num_accquired = 0
	num_slots = T * c
	for rounds in range(c):
		if unlocking_arbiter_continuous(CWmid, CWedge, delta):
			num_accquired += 1
		for i in range(1, T):
			if CSMA_arbiter_continuous(CWmid, CWedge):
				num_accquired += 1
	portion_accquired = num_accquired / num_slots * 100
	print("Continuous Simulation: accquired %d out of %d (%.2f%%) slots in %d rounds" \
			% (num_accquired, num_slots, portion_accquire))
	return

def calc_discrete_probility(CWmid, CWedge, delta, equal=False):
	'''
		CWmid, CWedge: in unit of slot time(20us)
		delta: in unit of 1us
		Assume that CWmid <= CWedge <= delta / 20
	'''
	if equal:
		delta_mid = DELTA_100us_PRE_CALCED_MID[CWmid][CWedge]
	else:
		delta_mid = delta
	C1 = CWmid + 1
	C2 = CWedge + 1
	P1 = (2 * C1 * C1 - 6 * C1 * C2 - 3 * C1 + 6 * C2 * C2 + 6 * C2 + 1) \
		 / (6 * C2 * C2)
	#now try to calculate P2
	P2 = 0
	for backoff_mid in range(C1):
		for artificial_backoff_mid in range(2 * delta_mid + 1):
			wait_time_mid = backoff_mid * 20 + artificial_backoff_mid
			#need to compute the possibility that wait_time_edge >= wait_time_mid
			p = 0	#the possibility that wait_time_edge >= wait_time_mid
			for backoff_edge in range(C2):
				tmp_p = 1 / C2 * min(1, max(0, (1 - (wait_time_mid - backoff_edge * 20) / (2 * delta + 1))))
				p += tmp_p
			P2 += 1 / C1 * 1 / (2 * delta_mid + 1) * p * p
	return P1, P2

def main():
	if len(sys.argv) < 4 or len(sys.argv) > 7:
		print('usage: %s <CWmid> <CWedge> [delta=100] [equal=0] [T=20] [c=100000]\n'
				'\tCWmid, CWedge\t: in unit of slot (20us)\n'
				'\tdelta\t\t: in unit of us\n'
				'\tequal\t\t: enable equal possibility at unlocking round' % sys.argv[0])
		exit()
	CWmid = int(sys.argv[1])
	CWedge = int(sys.argv[2])
	delta = int(sys.argv[3])
	equal = False
	T = 20
	c = 100000
	if len(sys.argv) >= 5:
		equal = int(sys.argv[4])
	if len(sys.argv) >= 6:
		T = int(sys.argv[5])
	if len(sys.argv) >= 7:
		c = int(sys.argv[6])
	P1, P2 = calc_discrete_probility(CWmid, CWedge, delta, equal)
	thoughput = (P2 * (1 - P1**T) / (1 - P1)) / T * 100
	print('calculated P1=%.2f%% P2=%.2f%%, thoughput %.2f' % (P1 * 100, P2 * 100, thoughput))
	test_probability_discrete(CWmid, CWedge, delta, equal)
	simulate_discrete(T, c, CWmid, CWedge, delta, equal)

if __name__ == '__main__':
	main()
