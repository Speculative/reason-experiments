type t('state, 'action) = {
  state: ref('state),
  reducer: ('state, 'action) => ('state, list('action))
};

let create = (state, reducer) => {
  state: ref(state),
  reducer: reducer
};

let dispatch = (s: t('state, 'action), a: 'action) => {
  let empty = ref(false);
  let actions = ref([a]);
  while (!empty^) {
    switch (actions^) {
    | [] => empty := true
    | [a', ..._] => {
        let (newState, newActions) = s.reducer(s.state^, a');
        s.state := newState;
        actions := List.concat([List.tl(actions^), newActions]);
      };
    };
  };
};

let getState = (s: t('state, 'action)) => s.state^;

let drainDispatch = (s: t('state, 'action), l: list('action)) => {
  l
  |> List.rev
  |> List.iter((a: 'action) => {
      dispatch(s, a);
    });
};

