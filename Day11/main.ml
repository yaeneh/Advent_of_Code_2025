let parse_line line =
  match String.index_opt line ':' with
  | None -> failwith "Missing ':'"
  | Some i ->
      let key = String.sub line 0 i |> String.trim in
      let rest =
        String.sub line (i + 1) (String.length line - i - 1)
        |> String.trim
      in
      let values =
        String.split_on_char ' ' rest
        |> List.filter (fun s -> s <> "")
      in
      (key, values)

let process_file filename =
  let ic = open_in filename in
  Fun.protect
    ~finally:(fun () -> close_in ic)
    (fun () ->
      let rec read_lines acc =
        match input_line ic with
        | line ->
            let pair = parse_line line in
            read_lines (pair :: acc)
        | exception End_of_file ->
            List.rev acc
            in
      read_lines [])

let rec dfs data current goal visited=
  if current = goal then
   1
  else if List.mem current visited then
   0
  else 
    let neighbours = List.assoc current data in
      List.fold_left (fun acc element -> acc + dfs data element goal (current::visited) ) 0 neighbours

let dfsp2 data start goal =
  let memo = Hashtbl.create 1000 in

  let rec dfs current seen_dac seen_fft =
    let seen_dac = seen_dac || current = "dac" in
    let seen_fft = seen_fft || current = "fft" in

    if current = goal then (
      if seen_dac && seen_fft then 1 else 0
    ) else (
      let key = (current, seen_dac, seen_fft) in

      match Hashtbl.find_opt memo key with
      | Some count -> count
      | None ->
          let neighbours =
            match List.assoc_opt current data with
            | Some nodes -> nodes
            | None -> []
          in

          let count =
            List.fold_left
              (fun acc next ->
                acc + dfs next seen_dac seen_fft)
              0
              neighbours
          in

          Hashtbl.replace memo key count;
          count
    )
  in

  dfs start false false

let () =
  let data = process_file "input" in
  let resultp1 = dfs data "you" "out" [] in
  Printf.printf "The result of p1 is: %d" resultp1;
  let resultp2 = dfsp2 data "svr" "out"  in 
  Printf.printf "result of p2 is: %d" resultp2

