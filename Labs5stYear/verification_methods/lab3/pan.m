#define rand	pan_rand
#define pthread_equal(a,b)	((a)==(b))
#if defined(HAS_CODE) && defined(VERBOSE)
	#ifdef BFS_PAR
		bfs_printf("Pr: %d Tr: %d\n", II, t->forw);
	#else
		cpu_printf("Pr: %d Tr: %d\n", II, t->forw);
	#endif
#endif
	switch (t->forw) {
	default: Uerror("bad forward move");
	case 0:	/* if without executable clauses */
		continue;
	case 1: /* generic 'goto' or 'skip' */
		IfNotBlocked
		_m = 3; goto P999;
	case 2: /* generic 'else' */
		IfNotBlocked
		if (trpt->o_pm&1) continue;
		_m = 3; goto P999;

		 /* PROC :init: */
	case 3: // STATE 1 - dining_philosophers_naive.pml:59 - [((i<5))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][1] = 1;
		if (!((((int)((P2 *)_this)->i)<5)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 4: // STATE 2 - dining_philosophers_naive.pml:60 - [(run fork(fork_left[i],fork_right[i]))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][2] = 1;
		if (!(addproc(II, 1, 0, now.fork_left[ Index(((int)((P2 *)_this)->i), 5) ], now.fork_right[ Index(((int)((P2 *)_this)->i), 5) ], 0)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 5: // STATE 3 - dining_philosophers_naive.pml:61 - [i = (i+1)] (0:0:1 - 1)
		IfNotBlocked
		reached[2][3] = 1;
		(trpt+1)->bup.oval = ((int)((P2 *)_this)->i);
		((P2 *)_this)->i = (((int)((P2 *)_this)->i)+1);
#ifdef VAR_RANGES
		logval(":init::i", ((int)((P2 *)_this)->i));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 6: // STATE 9 - dining_philosophers_naive.pml:66 - [i = 0] (0:15:1 - 3)
		IfNotBlocked
		reached[2][9] = 1;
		(trpt+1)->bup.oval = ((int)((P2 *)_this)->i);
		((P2 *)_this)->i = 0;
#ifdef VAR_RANGES
		logval(":init::i", ((int)((P2 *)_this)->i));
#endif
		;
		/* merge: .(goto)(0, 16, 15) */
		reached[2][16] = 1;
		;
		_m = 3; goto P999; /* 1 */
	case 7: // STATE 10 - dining_philosophers_naive.pml:68 - [((i<5))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][10] = 1;
		if (!((((int)((P2 *)_this)->i)<5)))
			continue;
		_m = 3; goto P999; /* 0 */
	case 8: // STATE 11 - dining_philosophers_naive.pml:69 - [(run phil(fork_right[(((i+5)-1)%5)],fork_left[i],i))] (0:0:0 - 1)
		IfNotBlocked
		reached[2][11] = 1;
		if (!(addproc(II, 1, 1, now.fork_right[ Index((((((int)((P2 *)_this)->i)+5)-1)%5), 5) ], now.fork_left[ Index(((int)((P2 *)_this)->i), 5) ], ((int)((P2 *)_this)->i))))
			continue;
		_m = 3; goto P999; /* 0 */
	case 9: // STATE 12 - dining_philosophers_naive.pml:70 - [i = (i+1)] (0:0:1 - 1)
		IfNotBlocked
		reached[2][12] = 1;
		(trpt+1)->bup.oval = ((int)((P2 *)_this)->i);
		((P2 *)_this)->i = (((int)((P2 *)_this)->i)+1);
#ifdef VAR_RANGES
		logval(":init::i", ((int)((P2 *)_this)->i));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 10: // STATE 18 - dining_philosophers_naive.pml:74 - [-end-] (0:0:0 - 3)
		IfNotBlocked
		reached[2][18] = 1;
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC phil */
	case 11: // STATE 1 - dining_philosophers_naive.pml:32 - [printf('MSC: phil # %d\\n',mn)] (0:0:0 - 1)
		IfNotBlocked
		reached[1][1] = 1;
		Printf("MSC: phil # %d\n", ((int)((P1 *)_this)->mn));
		_m = 3; goto P999; /* 0 */
	case 12: // STATE 2 - dining_philosophers_naive.pml:35 - [((cur_state==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][2] = 1;
		if (!((((int)((P1 *)_this)->cur_state)==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state */  (trpt+1)->bup.oval = ((P1 *)_this)->cur_state;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->cur_state = 0;
		_m = 3; goto P999; /* 0 */
	case 13: // STATE 3 - dining_philosophers_naive.pml:36 - [left!msgtype] (0:0:0 - 1)
		IfNotBlocked
		reached[1][3] = 1;
		if (q_len(((P1 *)_this)->left))
			continue;
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", ((P1 *)_this)->left);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(((P1 *)_this)->left, 0, 1, 1);
		{ boq = ((P1 *)_this)->left; };
		_m = 2; goto P999; /* 0 */
	case 14: // STATE 4 - dining_philosophers_naive.pml:37 - [cur_state = 2] (0:0:1 - 1)
		IfNotBlocked
		reached[1][4] = 1;
		(trpt+1)->bup.oval = ((int)((P1 *)_this)->cur_state);
		((P1 *)_this)->cur_state = 2;
#ifdef VAR_RANGES
		logval("phil:cur_state", ((int)((P1 *)_this)->cur_state));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 15: // STATE 5 - dining_philosophers_naive.pml:38 - [((cur_state==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][5] = 1;
		if (!((((int)((P1 *)_this)->cur_state)==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state */  (trpt+1)->bup.oval = ((P1 *)_this)->cur_state;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->cur_state = 0;
		_m = 3; goto P999; /* 0 */
	case 16: // STATE 6 - dining_philosophers_naive.pml:39 - [right!msgtype] (0:0:0 - 1)
		IfNotBlocked
		reached[1][6] = 1;
		if (q_len(((P1 *)_this)->right))
			continue;
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", ((P1 *)_this)->right);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(((P1 *)_this)->right, 0, 1, 1);
		{ boq = ((P1 *)_this)->right; };
		_m = 2; goto P999; /* 0 */
	case 17: // STATE 7 - dining_philosophers_naive.pml:40 - [cur_state = 3] (0:0:1 - 1)
		IfNotBlocked
		reached[1][7] = 1;
		(trpt+1)->bup.oval = ((int)((P1 *)_this)->cur_state);
		((P1 *)_this)->cur_state = 3;
#ifdef VAR_RANGES
		logval("phil:cur_state", ((int)((P1 *)_this)->cur_state));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 18: // STATE 8 - dining_philosophers_naive.pml:41 - [((cur_state==3))] (18:0:2 - 1)
		IfNotBlocked
		reached[1][8] = 1;
		if (!((((int)((P1 *)_this)->cur_state)==3)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state */  (trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->cur_state;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->cur_state = 0;
		/* merge: cur_state = 4(0, 9, 18) */
		reached[1][9] = 1;
		(trpt+1)->bup.ovals[1] = ((int)((P1 *)_this)->cur_state);
		((P1 *)_this)->cur_state = 4;
#ifdef VAR_RANGES
		logval("phil:cur_state", ((int)((P1 *)_this)->cur_state));
#endif
		;
		/* merge: .(goto)(0, 19, 18) */
		reached[1][19] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 19: // STATE 10 - dining_philosophers_naive.pml:43 - [((cur_state==4))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][10] = 1;
		if (!((((int)((P1 *)_this)->cur_state)==4)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state */  (trpt+1)->bup.oval = ((P1 *)_this)->cur_state;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->cur_state = 0;
		_m = 3; goto P999; /* 0 */
	case 20: // STATE 11 - dining_philosophers_naive.pml:44 - [right!msgtype] (0:0:0 - 1)
		IfNotBlocked
		reached[1][11] = 1;
		if (q_len(((P1 *)_this)->right))
			continue;
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", ((P1 *)_this)->right);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(((P1 *)_this)->right, 0, 1, 1);
		{ boq = ((P1 *)_this)->right; };
		_m = 2; goto P999; /* 0 */
	case 21: // STATE 12 - dining_philosophers_naive.pml:45 - [cur_state = 5] (0:0:1 - 1)
		IfNotBlocked
		reached[1][12] = 1;
		(trpt+1)->bup.oval = ((int)((P1 *)_this)->cur_state);
		((P1 *)_this)->cur_state = 5;
#ifdef VAR_RANGES
		logval("phil:cur_state", ((int)((P1 *)_this)->cur_state));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 22: // STATE 13 - dining_philosophers_naive.pml:46 - [((cur_state==5))] (0:0:1 - 1)
		IfNotBlocked
		reached[1][13] = 1;
		if (!((((int)((P1 *)_this)->cur_state)==5)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state */  (trpt+1)->bup.oval = ((P1 *)_this)->cur_state;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->cur_state = 0;
		_m = 3; goto P999; /* 0 */
	case 23: // STATE 14 - dining_philosophers_naive.pml:47 - [left!msgtype] (0:0:0 - 1)
		IfNotBlocked
		reached[1][14] = 1;
		if (q_len(((P1 *)_this)->left))
			continue;
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[64];
			sprintf(simvals, "%d!", ((P1 *)_this)->left);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		
		qsend(((P1 *)_this)->left, 0, 1, 1);
		{ boq = ((P1 *)_this)->left; };
		_m = 2; goto P999; /* 0 */
	case 24: // STATE 15 - dining_philosophers_naive.pml:48 - [cur_state = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[1][15] = 1;
		(trpt+1)->bup.oval = ((int)((P1 *)_this)->cur_state);
		((P1 *)_this)->cur_state = 0;
#ifdef VAR_RANGES
		logval("phil:cur_state", ((int)((P1 *)_this)->cur_state));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 25: // STATE 16 - dining_philosophers_naive.pml:49 - [((cur_state==0))] (18:0:2 - 1)
		IfNotBlocked
		reached[1][16] = 1;
		if (!((((int)((P1 *)_this)->cur_state)==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state */  (trpt+1)->bup.ovals = grab_ints(2);
		(trpt+1)->bup.ovals[0] = ((P1 *)_this)->cur_state;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P1 *)_this)->cur_state = 0;
		/* merge: cur_state = 1(0, 17, 18) */
		reached[1][17] = 1;
		(trpt+1)->bup.ovals[1] = ((int)((P1 *)_this)->cur_state);
		((P1 *)_this)->cur_state = 1;
#ifdef VAR_RANGES
		logval("phil:cur_state", ((int)((P1 *)_this)->cur_state));
#endif
		;
		/* merge: .(goto)(0, 19, 18) */
		reached[1][19] = 1;
		;
		_m = 3; goto P999; /* 2 */
	case 26: // STATE 21 - dining_philosophers_naive.pml:52 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[1][21] = 1;
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */

		 /* PROC fork */
	case 27: // STATE 1 - dining_philosophers_naive.pml:14 - [((cur_state_fork==0))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][1] = 1;
		if (!((((int)((P0 *)_this)->cur_state_fork)==0)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state_fork */  (trpt+1)->bup.oval = ((P0 *)_this)->cur_state_fork;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->cur_state_fork = 0;
		_m = 3; goto P999; /* 0 */
	case 28: // STATE 2 - dining_philosophers_naive.pml:16 - [right_phil?msgtype] (0:0:0 - 1)
		reached[0][2] = 1;
		if (boq != ((P0 *)_this)->right_phil) continue;
		if (q_len(((P0 *)_this)->right_phil) == 0) continue;

		XX=1;
		if (1 != qrecv(((P0 *)_this)->right_phil, 0, 0, 0)) continue;
		
#ifndef BFS_PAR
		if (q_flds[((Q0 *)qptr(((P0 *)_this)->right_phil-1))->_t] != 1)
			Uerror("wrong nr of msg fields in rcv");
#endif
		;
		qrecv(((P0 *)_this)->right_phil, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", ((P0 *)_this)->right_phil);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		if (q_zero(((P0 *)_this)->right_phil))
		{	boq = -1;
#ifndef NOFAIR
			if (fairness
			&& !(trpt->o_pm&32)
			&& (now._a_t&2)
			&&  now._cnt[now._a_t&1] == II+2)
			{	now._cnt[now._a_t&1] -= 1;
#ifdef VERI
				if (II == 1)
					now._cnt[now._a_t&1] = 1;
#endif
#ifdef DEBUG
			printf("%3ld: proc %d fairness ", depth, II);
			printf("Rule 2: --cnt to %d (%d)\n",
				now._cnt[now._a_t&1], now._a_t);
#endif
				trpt->o_pm |= (32|64);
			}
#endif

		};
		_m = 4; goto P999; /* 0 */
	case 29: // STATE 3 - dining_philosophers_naive.pml:16 - [cur_state_fork = 2] (0:0:1 - 1)
		IfNotBlocked
		reached[0][3] = 1;
		(trpt+1)->bup.oval = ((int)((P0 *)_this)->cur_state_fork);
		((P0 *)_this)->cur_state_fork = 2;
#ifdef VAR_RANGES
		logval("fork:cur_state_fork", ((int)((P0 *)_this)->cur_state_fork));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 30: // STATE 4 - dining_philosophers_naive.pml:17 - [left_phil?msgtype] (0:0:0 - 1)
		reached[0][4] = 1;
		if (boq != ((P0 *)_this)->left_phil) continue;
		if (q_len(((P0 *)_this)->left_phil) == 0) continue;

		XX=1;
		if (1 != qrecv(((P0 *)_this)->left_phil, 0, 0, 0)) continue;
		
#ifndef BFS_PAR
		if (q_flds[((Q0 *)qptr(((P0 *)_this)->left_phil-1))->_t] != 1)
			Uerror("wrong nr of msg fields in rcv");
#endif
		;
		qrecv(((P0 *)_this)->left_phil, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", ((P0 *)_this)->left_phil);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		if (q_zero(((P0 *)_this)->left_phil))
		{	boq = -1;
#ifndef NOFAIR
			if (fairness
			&& !(trpt->o_pm&32)
			&& (now._a_t&2)
			&&  now._cnt[now._a_t&1] == II+2)
			{	now._cnt[now._a_t&1] -= 1;
#ifdef VERI
				if (II == 1)
					now._cnt[now._a_t&1] = 1;
#endif
#ifdef DEBUG
			printf("%3ld: proc %d fairness ", depth, II);
			printf("Rule 2: --cnt to %d (%d)\n",
				now._cnt[now._a_t&1], now._a_t);
#endif
				trpt->o_pm |= (32|64);
			}
#endif

		};
		_m = 4; goto P999; /* 0 */
	case 31: // STATE 5 - dining_philosophers_naive.pml:17 - [cur_state_fork = 1] (0:0:1 - 1)
		IfNotBlocked
		reached[0][5] = 1;
		(trpt+1)->bup.oval = ((int)((P0 *)_this)->cur_state_fork);
		((P0 *)_this)->cur_state_fork = 1;
#ifdef VAR_RANGES
		logval("fork:cur_state_fork", ((int)((P0 *)_this)->cur_state_fork));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 32: // STATE 8 - dining_philosophers_naive.pml:19 - [((cur_state_fork==1))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][8] = 1;
		if (!((((int)((P0 *)_this)->cur_state_fork)==1)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state_fork */  (trpt+1)->bup.oval = ((P0 *)_this)->cur_state_fork;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->cur_state_fork = 0;
		_m = 3; goto P999; /* 0 */
	case 33: // STATE 9 - dining_philosophers_naive.pml:20 - [left_phil?msgtype] (0:0:0 - 1)
		reached[0][9] = 1;
		if (boq != ((P0 *)_this)->left_phil) continue;
		if (q_len(((P0 *)_this)->left_phil) == 0) continue;

		XX=1;
		if (1 != qrecv(((P0 *)_this)->left_phil, 0, 0, 0)) continue;
		
#ifndef BFS_PAR
		if (q_flds[((Q0 *)qptr(((P0 *)_this)->left_phil-1))->_t] != 1)
			Uerror("wrong nr of msg fields in rcv");
#endif
		;
		qrecv(((P0 *)_this)->left_phil, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", ((P0 *)_this)->left_phil);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		if (q_zero(((P0 *)_this)->left_phil))
		{	boq = -1;
#ifndef NOFAIR
			if (fairness
			&& !(trpt->o_pm&32)
			&& (now._a_t&2)
			&&  now._cnt[now._a_t&1] == II+2)
			{	now._cnt[now._a_t&1] -= 1;
#ifdef VERI
				if (II == 1)
					now._cnt[now._a_t&1] = 1;
#endif
#ifdef DEBUG
			printf("%3ld: proc %d fairness ", depth, II);
			printf("Rule 2: --cnt to %d (%d)\n",
				now._cnt[now._a_t&1], now._a_t);
#endif
				trpt->o_pm |= (32|64);
			}
#endif

		};
		_m = 4; goto P999; /* 0 */
	case 34: // STATE 10 - dining_philosophers_naive.pml:21 - [cur_state_fork = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][10] = 1;
		(trpt+1)->bup.oval = ((int)((P0 *)_this)->cur_state_fork);
		((P0 *)_this)->cur_state_fork = 0;
#ifdef VAR_RANGES
		logval("fork:cur_state_fork", ((int)((P0 *)_this)->cur_state_fork));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 35: // STATE 11 - dining_philosophers_naive.pml:22 - [((cur_state_fork==2))] (0:0:1 - 1)
		IfNotBlocked
		reached[0][11] = 1;
		if (!((((int)((P0 *)_this)->cur_state_fork)==2)))
			continue;
		if (TstOnly) return 1; /* TT */
		/* dead 1: cur_state_fork */  (trpt+1)->bup.oval = ((P0 *)_this)->cur_state_fork;
#ifdef HAS_CODE
		if (!readtrail)
#endif
			((P0 *)_this)->cur_state_fork = 0;
		_m = 3; goto P999; /* 0 */
	case 36: // STATE 12 - dining_philosophers_naive.pml:23 - [right_phil?msgtype] (0:0:0 - 1)
		reached[0][12] = 1;
		if (boq != ((P0 *)_this)->right_phil) continue;
		if (q_len(((P0 *)_this)->right_phil) == 0) continue;

		XX=1;
		if (1 != qrecv(((P0 *)_this)->right_phil, 0, 0, 0)) continue;
		
#ifndef BFS_PAR
		if (q_flds[((Q0 *)qptr(((P0 *)_this)->right_phil-1))->_t] != 1)
			Uerror("wrong nr of msg fields in rcv");
#endif
		;
		qrecv(((P0 *)_this)->right_phil, XX-1, 0, 1);
		
#ifdef HAS_CODE
		if (readtrail && gui) {
			char simtmp[32];
			sprintf(simvals, "%d?", ((P0 *)_this)->right_phil);
		sprintf(simtmp, "%d", 1); strcat(simvals, simtmp);		}
#endif
		if (q_zero(((P0 *)_this)->right_phil))
		{	boq = -1;
#ifndef NOFAIR
			if (fairness
			&& !(trpt->o_pm&32)
			&& (now._a_t&2)
			&&  now._cnt[now._a_t&1] == II+2)
			{	now._cnt[now._a_t&1] -= 1;
#ifdef VERI
				if (II == 1)
					now._cnt[now._a_t&1] = 1;
#endif
#ifdef DEBUG
			printf("%3ld: proc %d fairness ", depth, II);
			printf("Rule 2: --cnt to %d (%d)\n",
				now._cnt[now._a_t&1], now._a_t);
#endif
				trpt->o_pm |= (32|64);
			}
#endif

		};
		_m = 4; goto P999; /* 0 */
	case 37: // STATE 13 - dining_philosophers_naive.pml:24 - [cur_state_fork = 0] (0:0:1 - 1)
		IfNotBlocked
		reached[0][13] = 1;
		(trpt+1)->bup.oval = ((int)((P0 *)_this)->cur_state_fork);
		((P0 *)_this)->cur_state_fork = 0;
#ifdef VAR_RANGES
		logval("fork:cur_state_fork", ((int)((P0 *)_this)->cur_state_fork));
#endif
		;
		_m = 3; goto P999; /* 0 */
	case 38: // STATE 17 - dining_philosophers_naive.pml:26 - [-end-] (0:0:0 - 1)
		IfNotBlocked
		reached[0][17] = 1;
		if (!delproc(1, II)) continue;
		_m = 3; goto P999; /* 0 */
	case  _T5:	/* np_ */
		if (!((!(trpt->o_pm&4) && !(trpt->tau&128))))
			continue;
		/* else fall through */
	case  _T2:	/* true */
		_m = 3; goto P999;
#undef rand
	}

