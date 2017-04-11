#!/usr/bin/python3

import collections

def calc_P2(CWmid, CWedge, delta, delta_mid=None):
	'''
		CWmid, CWedge: in unit of slot time(20us)
		delta: in unit of 1us
		Assume that CWmid <= CWedge <= delta / 20
	'''
	if not delta_mid:
		delta_mid = delta
	C1 = CWmid + 1
	C2 = CWedge + 1
	P2 = 0
	for backoff_mid in range(C1):
		for artificial_backoff_mid in range(2 * delta_mid + 1):
			wait_time_mid = backoff_mid * 20 + artificial_backoff_mid
			#need to compute the possibility that wait_time_edge >= wait_time_mid
			p = 0   #the possibility that wait_time_edge >= wait_time_mid
			for backoff_edge in range(C2):
				tmp_p = 1 / C2 * min(1, \
					max(0, (1 - (wait_time_mid - backoff_edge * 20) / (2 * delta + 1))))
				p += tmp_p
			P2 += 1 / C1 * 1 / (2 * delta_mid + 1) * p * p
	return P2

def calc_delta_mid(CWmid, CWedge, delta):
	'''
		CWmid, CWedge: in unit of slot (20us)
		delta: in unit of delta
		Assume that CWmid <= CWedge <= delta / 20
		this function returns the best integer value of delta_mid such that
		the probability of mid node acquiring the channel is close to 1/3
	'''
	target_P = 1 / 3
	left = delta - 10
	right = left + 1
	P_diff = 1
	while P_diff > 0:
		P2 = calc_P2(CWmid, CWedge, delta, right)
		P_diff = P2 - target_P
		right += 10000
	#P2 and P_diff should monotonically decrease as delta_mid increases
	#which makes it possible to use binary search
	while left < right:
		mid = (left + right) // 2
		P2 = calc_P2(CWmid, CWedge, delta, mid)
		P_diff = P2 - target_P
		if P_diff > 0:
			left = mid + 1
		else:
			right = mid
	P2 = calc_P2(CWmid, CWedge, delta, right)
	P2_1 = calc_P2(CWmid, CWedge, delta, right - 1)
	if P2_1 < target_P:
		print('this shall never happen')
	if P2 > target_P:
		print('this shall never happen')
	if P2_1 - target_P < target_P - P2:
		return right - 1
	else:
		return right

def main():
	delta = 100
	ret = collections.OrderedDict()
	for i in range(11):
		CWmid = 2 ** i - 1
		ret[CWmid] = collections.OrderedDict()
		for j in range(i, 11):
			CWedge = 2 ** j - 1
			ret[CWmid][CWedge] = calc_delta_mid(CWmid, CWedge, delta)
			print('CWmid=%d CWedge=%d Calculated delta_mid=%d' % (CWmid, CWedge, ret[CWmid][CWedge]))
	print(ret)

if __name__ == '__main__':
	main()
