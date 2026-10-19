type shape = {
  mutable id : int;
  cells : bool list list;
}

type region = {
  width : int;
  height : int;
  counts : int list;
}

type parse_state =
  | Inbetween
  | Reading_shape of int * bool list list
  | Regions


let parse_grid_row line =
  line
  |> String.to_seq
  |> List.of_seq
  |> List.map (fun c -> c = '#')


let parse_header line =
  match String.split_on_char ':' line with
  | [header; ""] ->
      (match String.split_on_char 'x' header with
       | [id] -> `Shape (int_of_string id)
       | [w; h] -> `Region (int_of_string w, int_of_string h)
       | _ -> failwith "Invalid header")
  | _ -> failwith "Invalid header"


let parse_region line =
  match String.split_on_char ':' line with
  | [dims; values] ->
      (match String.split_on_char 'x' dims with
       | [w; h] ->
           {
             width = int_of_string w;
             height = int_of_string h;
             counts =
               String.split_on_char ' ' (String.trim values)
               |> List.filter (fun s -> s <> "")
               |> List.map int_of_string;
           }
       | _ -> failwith "Invalid region dimensions")
  | _ -> failwith "Invalid region"


let finish_shape current shapes =
  match current with
  | None -> shapes
  | Some (id, rows) ->
      { id; cells = List.rev rows } :: shapes


let process_lines ic =
  let rec loop current shapes regions =
    match input_line ic with
    | exception End_of_file ->
        List.rev (finish_shape current shapes), List.rev regions

    | line ->
        let line = String.trim line in

        if line = "" then
          loop current shapes regions

        else if String.ends_with ~suffix:":" line then
          (match parse_header line with
           | `Shape id ->
               let shapes = finish_shape current shapes in
               loop (Some (id, [])) shapes regions

           | `Region _ ->
               failwith "Expected region values after dimensions")

        else if String.contains line ':' then
          let shapes = finish_shape current shapes in
          let region = parse_region line in
          loop None shapes (region :: regions)

        else
          match current with
          | Some (id, rows) ->
              let row = parse_grid_row line in
              loop (Some (id, row :: rows)) shapes regions
          | None ->
              failwith ("Unexpected line: " ^ line)
  in
  loop None [] []

let process_file filename =
  let ic = open_in filename in
  Fun.protect
    ~finally:(fun () -> close_in ic)
    (fun () -> process_lines ic)

let countsize (cells : bool list list) =
  List.fold_left
    (fun total row ->
      total +
      List.fold_left
        (fun count cell -> if cell then count + 1 else count)
        0 row)
    0 cells

let present_size region shapes =
  List.map2
    (fun amount shape -> amount * shape.id)
    region.counts shapes

let () =
  let shapes, regions = process_file "input" in

  Printf.printf "Shapes: %d\n" (List.length shapes);
  Printf.printf "Regions: %d\n" (List.length regions);

  List.iter
    (fun shape -> shape.id <- countsize shape.cells)
    shapes;

  let maxcoverage =
    List.map
      (fun region ->
        List.fold_left (+) 0 (present_size region shapes))
      regions
  in

  let space =
    List.map
      (fun region -> region.width * region.height)
      regions
  in

  let result =
    List.map2
      (fun coverage space -> if coverage <= space then 1 else 0)
      maxcoverage space
  in

  let nr = List.fold_left (+) 0 result in

  Printf.printf "Result of part1: %d\n" nr
