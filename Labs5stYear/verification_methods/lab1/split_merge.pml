  #define size 16
  #define N 128
  chan ch    = [size] of { short };
  chan large = [size] of { short };
  chan small = [size] of { short };
  proctype split()
  {
      short data;
  S5: do
      :: ch ? data ->
          if
          :: (data >= N) -> large ! data
          :: else     -> small ! data
          fi
      od
  }
  proctype merge()
  {
      short data;
  S6: do
  :: if 
      :: large ? data;
      :: small ? data;
  fi
  ch ! data;
      od
  }
  init {
      ch ! 130; ch ! 12;
      run split();
      run merge();
  }
