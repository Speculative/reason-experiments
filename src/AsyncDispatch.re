let asyncDispatchQueue: ref(list(Actions.actions)) = ref([]);

let asyncDispatch = (a: Actions.actions) => {
  asyncDispatchQueue := [a, ...asyncDispatchQueue^];
};

let getDispatch = () => asyncDispatchQueue^;

let clearDispatch = () => {
  asyncDispatchQueue := [];
};
